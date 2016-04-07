Imports System.Threading
Imports System
Imports System.Text
Imports CyUSB

Public Class Form1

    Const VID = &H4B4
    Const PID = &HF0

    Dim usbDevices As New USBDeviceList(CyConst.DEVICES_CYUSB)              ' dynamic list of USBdevcies bound to CyUSB.sys
    Dim myDevice As CyUSBDevice                                             ' myDevice represents the bulkloop device in this example
    Dim dev As USBDevice
    Dim BulkInEndPt As CyBulkEndPoint                                       'We need one Bulk IN endpoint
    Dim BulkOutEndPt As CyBulkEndPoint                                      'We need one Bulk OUT endpoint

    Private txfers As Thread                                                'Thread where data transfers are handled
    Private txfersTx As Thread                                                'Thread where data transfers are handled
    Dim bConnecting As Boolean = False                                         'Variable determining whether tXfers thread is running or not
    Dim bRunning As Boolean = False                                         'Variable determining whether tXfers thread is running or not
    Dim bRunningTx As Boolean = False                                         'Variable determining whether tXfers thread is running or not
    Dim bRunningRxed As Boolean = False                                         'Variable determining whether tXfers thread is running or not
    Dim TextBoxTx1Len As Integer = 0

    Dim value, value1, value2, value3 As Integer
    Dim valueString As String
    Dim valueBytes As Byte()

    Dim ConfigStr As New List(Of String)
    Dim ConfigFileDir As String = "d:\_config\config.dat"

    Dim byteCount As Integer = 0
    Dim outCount, inCount As Long                                           'Number of bytes transferred in out and in direction 
    Dim failCount, passCount As Long                                        'Number of failed and passed transfers 
    Dim TxCount, RxCount As Long                                        'Number of failed and passed transfers 
    'Dim XFERSIZE As Integer = 1024                                          'Transfers happen in chunks of 512 bytes in case of High speed; and 1024 bytes in case of superspeed
    Dim XFERSIZE As Integer = 512

    Dim outData(0 To XFERSIZE - 1) As Byte                                  'OUT transfers send data in the outData buffer
    Dim inData(0 To XFERSIZE - 1) As Byte                                   'IN transfers send data into the inData buffer
    Dim inDataStr(0 To XFERSIZE - 1) As Byte                                   'IN transfers send data into the inData buffer

    Private Delegate Sub UpdateUICallBack()                                 'Setup the callback routine for updating the UI
    Dim updateUI As UpdateUICallBack

    'Delegates for updating Textbox from therad other than the parent thread
    Delegate Sub UpdateTextBoxDelegate(ByVal myString As String, ByVal txtBox As TextBox)
    Dim updateTxtBox As UpdateTextBoxDelegate = New UpdateTextBoxDelegate(AddressOf UpdateTextBox)

    Delegate Sub UpdateTextBoxDelegate1(ByVal myString As String, ByVal txtBox As TextBox)
    Dim updateTxtBox1 As UpdateTextBoxDelegate = New UpdateTextBoxDelegate(AddressOf UpdateTextBox1)

    Dim TimeCounter As Integer = 0
    Dim RxRateValue As Single = 0


    'Summary       Event handler for Form Load event
    Friend Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

        'Declaration of event handlers - Deviceattached and DeviceRemoved 
        AddHandler usbDevices.DeviceAttached, AddressOf usbDevices_DeviceAttached
        AddHandler usbDevices.DeviceRemoved, AddressOf usbDevices_DeviceRemoved

        'RefreshDevice()
        updateUI = New UpdateUICallBack(AddressOf StatusUpdate)             'Setup the callback routine for updating the UI
        'txfers = New Thread(AddressOf TransfersThread)
        'txfers.IsBackground = True
        'txfers.Priority = ThreadPriority.Highest

        FileTextbox.Text = ConfigFileDir
        ConnectBT.BackColor = Color.Aquamarine
        TxBt.Enabled = False
        StartBtn.Enabled = False
    End Sub

    Private Sub ConnectBT_Click(sender As Object, e As EventArgs) Handles ConnectBT.Click
        If (bConnecting = False) Then
            StartBtn.Enabled = True
            StartBtn.BackColor = Color.Aquamarine
            TxBt.Enabled = True
            RefreshDevice()
            'updateUI = New UpdateUICallBack(AddressOf StatusUpdate)             'Setup the callback routine for updating the UI
            txfers = New Thread(AddressOf TransfersThread)
            txfers.IsBackground = True
            txfers.Priority = ThreadPriority.Highest
            TextBoxRx1.Text = ""

            ConnectBT.Text = "DISCONNECT"
            ConnectBT.BackColor = Color.Pink
            bConnecting = True
        Else
            StartBtn.Enabled = False
            StartBtn.BackColor = Color.Pink
            TxBt.Enabled = False
            ConnectBT.Text = "CONNECT"
            ConnectBT.BackColor = Color.Aquamarine
            bConnecting = False
        End If
    End Sub

    'Summary
    '       Event handler for device attachment event
    Public Function usbDevices_DeviceAttached(ByVal sender As Object, ByVal e As EventArgs) As Integer
        RefreshDevice()
        Return Nothing
    End Function

    'Summary
    '       Event handler for device removal event
    Public Function usbDevices_DeviceRemoved(ByVal sender As Object, ByVal e As EventArgs) As Integer
        RefreshDevice()
        Return Nothing
    End Function

    ' Summary
    '       The function gets the device with VID=04b4 and PID=00F0; updates the Status label and Treeview
    Public Function RefreshDevice() As Integer

        myDevice = usbDevices(VID, PID)                                'Instantiating myDevice with handle to the device of interest (bulkloop device in this example)

        If myDevice Is Nothing Then                                         'Bulkloop device if not found
            StatusLabel.Text = "Bulkloop Device not connected"
            StartBtn.Enabled = False
            StartBtn.Text = "Start"
            StartBtn.BackColor = Color.Aquamarine
        Else                                                                'Bulkloop device if found
            StatusLabel.Text = "Bulkloop Device connected"
            StartBtn.Enabled = True
            BulkOutEndPt = myDevice.EndPointOf(&H2)                         'Instantiating the endpoints
            BulkInEndPt = myDevice.EndPointOf(&H86)
        End If

        'Clear and update TreeView
        DeviceTreeView.Nodes.Clear()
        For Each Me.dev In usbDevices
            DeviceTreeView.Nodes.Add(dev.Tree)
        Next
        Return Nothing
    End Function

    'Summary
    '       Event handler for Click event of Start/ Stop button. Starts or stops the thread and accordingly updates the text of the StartBtn.
    Private Sub StartBtn_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StartBtn.Click

        If ((BulkOutEndPt Is Nothing) And (BulkInEndPt Is Nothing)) Then                                        ' check for NULL endpoints
            MsgBox("No EP2 BULK OUT and EP6 BULK IN endpoints")
            StartBtn.Text = "Start"
            StartBtn.BackColor = Color.Aquamarine
            GoTo my_label
        Else
            If (BulkOutEndPt Is Nothing) Then
                MsgBox("No EP2 BULK OUT endpoint")
                StartBtn.Text = "Start"
                StartBtn.BackColor = Color.Aquamarine
                GoTo my_label
            End If
            If (BulkInEndPt Is Nothing) Then
                MsgBox("No EP6 BULK IN endpoint")
                StartBtn.Text = "Start"
                StartBtn.BackColor = Color.Aquamarine
                GoTo my_label
            End If
        End If

        If bRunning = False And bConnecting = True Then
            If (ValBox.Text <> "") Then
                'Dim encoding As Encoding = Encoding.UTF7
                Dim encoding As Encoding = Encoding.UTF8
                Dim byteArray As Byte() = encoding.GetBytes(ValBox.Text)
                byteCount = encoding.GetByteCount(ValBox.Text)
                value = 0
                value1 = 0
                value2 = 0
                value3 = 0
                If (byteCount >= 1) Then
                    value = Convert.ToInt32(encoding.GetString(byteArray, 0, 1), 16)
                End If
                If (byteCount >= 2) Then
                    value = Convert.ToInt32(encoding.GetString(byteArray, 0, 2), 16)
                End If
                If (byteCount >= 3) Then
                    value1 = Convert.ToInt32(encoding.GetString(byteArray, 2, 1), 16)
                End If
                If (byteCount >= 4) Then
                    value1 = Convert.ToInt32(encoding.GetString(byteArray, 2, 2), 16)
                End If
                If (byteCount >= 5) Then
                    value2 = Convert.ToInt32(encoding.GetString(byteArray, 4, 1), 16)
                End If
                If (byteCount >= 6) Then
                    value2 = Convert.ToInt32(encoding.GetString(byteArray, 4, 2), 16)
                End If
                If (byteCount >= 7) Then
                    value3 = Convert.ToInt32(encoding.GetString(byteArray, 6, 1), 16)
                End If
                If (byteCount >= 8) Then
                    value3 = Convert.ToInt32(encoding.GetString(byteArray, 6, 2), 16)
                End If
            End If

            outCount = 0
            inCount = 0
            failCount = 0
            passCount = 0
            TimeCounter = 0

            OutdataBox.Text = ""
            InDataBox.Text = ""

            bRunning = True
            StartBtn.Text = "Stop"
            StartBtn.BackColor = Color.Pink

            'creates new thread
            txfers = New Thread(AddressOf TransfersThread)
            txfers.IsBackground = True
            txfers.Priority = ThreadPriority.Highest
            'Starts the new thread
            txfers.Start()

            Timer1.Interval = 250
            Timer1.Enabled = True
            Timer1.Start()

        Else
            'Makes the thread stop and aborts the thread
            bRunning = False
            StartBtn.Text = "Start"
            StartBtn.BackColor = Color.Aquamarine

            If (txfers Is Nothing) Then
                Return
            End If

            If (txfers.IsAlive) Then
                txfers.Abort()
                txfers.Join()
                txfers = Nothing
                BulkOutEndPt.Reset()
            End If

            Timer1.Enabled = False
        End If
my_label:
        
    End Sub

    'Summary
    '       This thread is initiated on start button click. Runs the thread; invokes the data transfers and invokes the StatusUpdate to update the UI 
    Private Sub TransfersThread()
        Dim XferLen As Integer = XFERSIZE
        Dim bResult As Boolean = True
        If (myDevice.bSuperSpeed) Then
            XFERSIZE = 1024
        ElseIf (myDevice.bHighSpeed) Then
            XFERSIZE = 512
        End If
        XFERSIZE = 512

        While (bRunning = True And bResult = True)
            SetOutputData()                     'this function sets the bytes in outData to be transferred to OUT EP
            XferLen = XFERSIZE
            'calls the XferData function for bulk transfer(OUT) in the cyusb.dll
            bResult = BulkOutEndPt.XferData(outData, XferLen)
            outCount += XferLen

            If (bResult) Then
                'calls the XferData function for bulk transfer(IN) in the cyusb.dll
                bResult = BulkInEndPt.XferData(inData, XferLen)
                inCount += XferLen
            End If

            passCount = passCount + 1

            For i As Integer = 0 To XferLen - 1
                If (outData(i) <> inData(i)) Then
                    failCount = failCount + 1
                    passCount = passCount - 1
                    break()
                End If
            Next

            updateUI.Invoke()
        End While
        updateUI.Invoke()
my_label1:
        bRunning = False

    End Sub

    Private Sub TransfersThreadTx()
        Dim XferLen As Integer = XFERSIZE
        Dim bResulta As Boolean = False
        Dim bResultb As Boolean = False

        XFERSIZE = 512

        'While (bRunningTx = True And bResult = True)
        If (bRunningTx = True) Then
            SetOutputData5()


            'BulkInEndPt.Reset()

            XferLen = XFERSIZE
            'calls the XferData function for bulk transfer(OUT) in the cyusb.dll
            bResulta = BulkOutEndPt.XferData(outData, XferLen)
            outCount += XferLen
            bRunningTx = False

            'If (txfersTx Is Nothing) Then
            'Return
            'End If
            'If (txfersTx.IsAlive And 0) Then
            'txfersTx.Abort()
            'txfersTx.Join()
            'txfersTx = Nothing
            'BulkOutEndPt.Reset()
            'End If 

            If (bResulta) Then
                'calls the XferData function for bulk transfer(IN) in the cyusb.dll
                'bResultb = BulkInEndPt.XferData(inData, XferLen)
                While (bResultb = False)
                    bResultb = BulkInEndPt.XferData(inData, XferLen)
                End While

                inCount += XferLen
                bRunningRxed = True
                bResulta = False
            End If
            passCount = passCount + 1
            'RxCount = RxCount + 1

            updateUI.Invoke()
            'End While
        End If


        updateUI.Invoke()
my_label1:
        bRunningTx = False

    End Sub

    'Summary
    '       Takes the user-given data in the ‘Data Bytes(Hex)’ field of the application user interface and fills the ‘outData[]’ buffer with these bytes

    Private Function SetOutputData() As Integer
        For i As Integer = 0 To XFERSIZE - 1

            If ((byteCount >= 1) And (i < XFERSIZE)) Then
                outData(i) = CByte(value)
            End If
            If ((byteCount >= 3) And (i < XFERSIZE)) Then
                i = i + 1
                outData(i) = CByte(value1)
            End If
            If ((byteCount >= 5) And (++i < XFERSIZE)) Then
                i = i + 1
                outData(i) = CByte(value2)
            End If
            If ((byteCount >= 7) And (++i < XFERSIZE)) Then
                i = i + 1
                outData(i) = CByte(value3)
            End If
        Next
        Return Nothing
    End Function

    Private Function SetOutputData2() As Integer
        'For i As Integer = 0 To XFERSIZE - 1 - 8
        Dim i As Integer = 0
        outData(i) = CByte(value)
        i = i + 1
        outData(i) = CByte(value1)
        i = i + 1
        outData(i) = CByte(value2)
        i = i + 1
        outData(i) = CByte(value3)
        i = i + 1
        'Next

        For j As Integer = 8 To XFERSIZE - 1
            outData(j) = 0
        Next

        Return Nothing
    End Function

    Private Function SetOutputData3() As Integer
        'For i As Integer = 0 To XFERSIZE - 1 - 8
        Dim i As Integer = 0
        outData(i) = 1
        i = i + 1
        outData(i) = 2
        i = i + 1
        outData(i) = 3
        i = i + 1
        outData(i) = 4
        i = i + 1
        'Next

        For j As Integer = 8 To XFERSIZE - 1
            outData(j) = 0
        Next

        Return Nothing
    End Function

    Private Function SetOutputData5() As Integer
        'For i As Integer = 0 To 8 - 1
        If (TextBoxTx1Len <= XFERSIZE) Then
            For i As Integer = 0 To TextBoxTx1Len - 1
                outData(i) = valueBytes(i)
            Next
        End If

        If (TextBoxTx1Len <= XFERSIZE) Then
            'For j As Integer = 8 To XFERSIZE - 1
            For j As Integer = TextBoxTx1Len To XFERSIZE - 1
                outData(j) = 0
            Next
        End If

        Return Nothing
    End Function

    'Summary
    '       This is the call back function for updating the UI(user interface) and is called from TransfersThread. This updates the OutdataBox,InDataBox
    '       with the Bytes transferred; and NumBytesOut,NumBytesIn with the number of Bytes transferred.
    Private Sub StatusUpdate()
        Dim dataStrOut As New System.Text.StringBuilder("")
        Dim dataStrIn As New System.Text.StringBuilder("")

        If (bRunning) Then
            For i As Integer = 0 To XFERSIZE - 1
                If ((i Mod 16) = 0) Then
                    dataStrOut.AppendFormat(vbNewLine & "{0:X4}  ", i)
                End If
                dataStrOut.AppendFormat("{0:X2} ", outData(i))
            Next
            dataStrOut.AppendFormat(vbNewLine)
            Me.Invoke(updateTxtBox, dataStrOut.ToString, OutdataBox)


            For i As Integer = 0 To XFERSIZE - 1
                If ((i Mod 16) = 0) Then
                    dataStrIn.AppendFormat(vbNewLine & "{0:X4}  ", i)
                End If
                dataStrIn.AppendFormat("{0:X2} ", inData(i))
            Next
            dataStrIn.AppendFormat(vbNewLine)
            Me.Invoke(updateTxtBox, dataStrIn.ToString, InDataBox)
        End If


        'ADD
        If (bRunningRxed And 1) Then
            Dim strText As String = ""
            Dim strTextFilter As String = ""
            bRunningRxed = False

            '31-32-33-34-35..... 
            'strText = BitConverter.ToString(inData)  


            For i As Integer = 0 To XFERSIZE - 1
                strText = strText + Chr(inData(i))
            Next
            strTextFilter = Replace(strText, vbNullChar, "")
            strTextFilter = Replace(strTextFilter, vbCrLf, "")
            RxCount = RxCount + Len(strTextFilter)

            Me.Invoke(updateTxtBox1, strTextFilter, TextBoxRx1)
            Me.Invoke(updateTxtBox1, RxCount.ToString, RxNumTb1)

        End If
        'ADD end 


        Me.Invoke(updateTxtBox1, TimeCounter.ToString, RxTimerTb)
        Me.Invoke(updateTxtBox1, RxRateValue.ToString, RxRateTb)

        Me.Invoke(updateTxtBox1, outCount.ToString, NumBytesOut)
        Me.Invoke(updateTxtBox1, inCount.ToString, NumBytesIn)
        Me.Invoke(updateTxtBox1, failCount.ToString, NumTransfersFailed)
        Me.Invoke(updateTxtBox1, passCount.ToString, NumTransfersPassed)
        'StartBtn.Text = IIf(bRunning, "Stop", "Start")
        'StartBtn.BackColor = IIf(bRunning, Color.Pink, Color.Aquamarine)

    End Sub

    'Summary
    '       Delegate for updating text box from a thread other than the parent thread
    Public Sub UpdateTextBox(ByVal myString As String, ByVal txtBox As TextBox)
        txtBox.AppendText(myString)
    End Sub

    Private Sub TxBt_Click(sender As Object, e As EventArgs) Handles TxBt.Click

        If bRunningTx = False Then
            If (TextBoxTx1.Text <> "" And bConnecting = True) Then
                ' Dim encoding As Encoding = Encoding.UTF7
                Dim encoding As Encoding = Encoding.UTF8
                Dim byteArray As Byte() = encoding.GetBytes(TextBoxTx1.Text)

                byteCount = encoding.GetByteCount(TextBoxTx1.Text)
                'value = 0
                ' Dim valueString As String
                ' Dim valueBytes As Byte()

                TextBoxTx1Len = TextBoxTx1.TextLength
                'valueBytes = encoding.GetBytes(TextBoxTx1.Text, 0, 8)
                valueBytes = encoding.GetBytes(TextBoxTx1.Text, 0, TextBoxTx1Len)


                'If (byteCount >= 1) Then
                'value = Convert.ToInt32(encoding.GetString(byteArray, 0, 1), 16)
                'End If
                'If (byteCount >= 2) Then
                'value = Convert.ToInt32(encoding.GetString(byteArray, 0, 2), 16)
                'End If
            Else
                'Dim encoding As Encoding = Encoding.UTF7
                Dim encoding As Encoding = Encoding.UTF8
                Dim byteArray As Byte() = encoding.GetBytes(" ")
                byteCount = encoding.GetByteCount(TextBoxTx1.Text)
                TextBoxTx1Len = 0
                valueBytes = encoding.GetBytes(" ", 0, TextBoxTx1Len)
            End If

            'outCount = 0
            'inCount = 0
            'failCount = 0
            'passCount = 0 
            'OutdataBox.Text = ""
            'InDataBox.Text = ""

            TxCount = TxCount + Len(TextBoxTx1.Text)
            TxNumTb1.Text = TxCount.ToString

            bRunningTx = True

            'creates new thread
            txfersTx = New Thread(AddressOf TransfersThreadTx)
            txfersTx.IsBackground = True
            txfersTx.Priority = ThreadPriority.Highest
            txfersTx.Start()

        Else
            'Makes the thread stop and aborts the thread
            bRunningTx = False

            If (txfersTx Is Nothing) Then
                Return
            End If
            If (txfersTx.IsAlive) Then
                txfersTx.Abort()
                txfersTx.Join()
                txfersTx = Nothing
                BulkOutEndPt.Reset()
            End If
        End If

my_label:

    End Sub

    Private Sub StatusLabel_Click(sender As Object, e As EventArgs) Handles StatusLabel.Click

    End Sub

    Private Sub ValBox_TextChanged(sender As Object, e As EventArgs) Handles ValBox.TextChanged

    End Sub

    Private Sub ClearRxBt_Click(sender As Object, e As EventArgs) Handles ClearRxBt.Click

        RxCount = 0
        TextBoxRx1.Text = ""
        RxNumTb1.Text = RxCount.ToString

        'inDataRx = Nothing 
    End Sub

    Private Sub ClearTxBt_Click(sender As Object, e As EventArgs) Handles ClearTxBt.Click
        TextBoxTx1.Text = ""

        TxCount = 0
        TxNumTb1.Text = TxCount.ToString
    End Sub

    Private Sub ToolStripMenuItem1_Click(sender As Object, e As EventArgs) Handles SaveToolStripMenuItemToolStripMenuItem.Click
        If IO.File.Exists(ConfigFileDir) Then
            'Delete the file to recycle
            'My.Computer.FileSystem.DeleteFile(ConfigFileDir, FileIO.UIOption.OnlyErrorDialogs, FileIO.RecycleOption.SendToRecycleBin, FileIO.UICancelOption.DoNothing)
            'Delete the file from disk
            IO.File.Delete(ConfigFileDir)
        End If

        Dim file As New System.IO.StreamWriter(ConfigFileDir, True)
        'file.WriteLine(vbNewLine)
        file.WriteLine(TextBoxTxCmd.Text)
        file.Close()

    End Sub

    Private Sub ToolStripMenuItem1_Click_1(sender As Object, e As EventArgs)

        Dim file As New System.IO.StreamReader(ConfigFileDir)
        Dim words As String
        Dim words_nospace As String
        Dim i As Integer = 0

        If (file Is Nothing) Then
            GoTo subendd
        End If

        If (1) Then
            Do While file.Peek > -1 'until the end of the file
                words = file.ReadLine
                words_nospace = Replace(words, " ", "")
                words_nospace = Replace(words_nospace, "*", "")
                words_nospace = Replace(words_nospace, "?", "")
                ConfigStr.Add(words_nospace)
                TextBoxTxCmd.Text = TextBoxTxCmd.Text & words_nospace & vbCrLf      ' 
                'TextBoxTx1.Text = TextBoxTx1.Text & words & vbCrLf
                'ListViewTxCmd.Items.Add(configStr(i)) '= words & vbCrLf
                i = i + 1
            Loop
        End If

        file.Close()
subendd:
        i = 0
    End Sub

    Private Sub TxCmdCopyCMD_Click(sender As Object, e As EventArgs) Handles TxCmdCopyCMD.Click
        Dim StrCmd As String = TextBoxTxCmd.SelectedText
        Dim StrCmd1 As String
        StrCmd1 = Replace(StrCmd, " ", "")
        StrCmd1 = Replace(StrCmd1, vbCrLf, "")
        TextBoxTx1.Text = StrCmd1
    End Sub

    Private Sub TxCmdClrCMD_Click(sender As Object, e As EventArgs) Handles TxCmdClrCMD.Click
        TextBoxTxCmd.Text = ""
    End Sub

    Private Sub FileUpCmd_Click(sender As Object, e As EventArgs) Handles FileUpCmd.Click
        ConfigFileDir = FileTextbox.Text
    End Sub

    Private Sub label2_Click(sender As Object, e As EventArgs) Handles label2.Click

    End Sub

    Private Sub ToolStripMenuItem1_Click_2(sender As Object, e As EventArgs) Handles ToolStripMenuItem1.Click

        Dim file As New System.IO.StreamReader(ConfigFileDir)
        Dim words As String
        Dim words_nospace As String
        Dim i As Integer = 0

        If (file Is Nothing) Then
            GoTo subendd
        End If

        If (1) Then
            Do While file.Peek > -1 'until the end of the file
                words = file.ReadLine
                words_nospace = Replace(words, " ", "")
                words_nospace = Replace(words_nospace, "*", "")
                'words_nospace = Replace(words_nospace, "?", "")
                'words_nospace = Replace(words_nospace, "{", "[")
                ConfigStr.Add(words_nospace)
                TextBoxTxCmd.Text = TextBoxTxCmd.Text & words_nospace & vbCrLf      ' 
                'TextBoxTx1.Text = TextBoxTx1.Text & words & vbCrLf
                'ListViewTxCmd.Items.Add(configStr(i)) '= words & vbCrLf
                i = i + 1
            Loop
        End If

        file.Close()
subendd:
        i = 0
    End Sub

    Private Sub aaa_Click(sender As Object, e As EventArgs)

    End Sub


    Private Sub RECONNECTToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles RECONNECTToolStripMenuItem.Click
        myDevice = usbDevices(VID, PID)                                'Instantiating myDevice with handle to the device of interest (bulkloop device in this example)

        If myDevice Is Nothing Then
            Return
        Else
            myDevice.ReConnect()
        End If
    End Sub

    Private Sub RESETToolStripMenuItem_Click(sender As Object, e As EventArgs) Handles RESETToolStripMenuItem.Click
        myDevice = usbDevices(VID, PID)                                'Instantiating myDevice with handle to the device of interest (bulkloop device in this example)

        If myDevice Is Nothing Then
            Return
        Else
            myDevice.Reset()
            RefreshDevice()
        End If
    End Sub

    Private Sub InDataBox_TextChanged(sender As Object, e As EventArgs) Handles InDataBox.TextChanged

    End Sub

    Private Sub Label8_Click(sender As Object, e As EventArgs) Handles Label8.Click

    End Sub

    Private Sub label3_Click(sender As Object, e As EventArgs) Handles label3.Click

    End Sub

    Private Sub RxNumTb1_TextChanged(sender As Object, e As EventArgs) Handles RxNumTb1.TextChanged

    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles TranClrBt.Click

        outCount = 0
        inCount = 0
        failCount = 0
        passCount = 0
        TimeCounter = 0

        OutdataBox.Text = ""
        InDataBox.Text = ""
        NumBytesOut.Text = "0"
        NumTransfersPassed.Text = "0"
        NumBytesIn.Text = "0"
        NumTransfersFailed.Text = "0"
        RxRateTb.Text = "0"
        RxTimerTb.Text = "0"
    End Sub

    'Summary
    '       Delegate for updating text box from a thread other than the parent thread
    Public Sub UpdateTextBox1(ByVal myString As String, ByVal txtBox As TextBox)
        txtBox.Text = myString
    End Sub

    'Summary
    '       Eventhandler for Form1 Close event
    Private Sub Form1_FormClosing(ByVal sender As System.Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles MyBase.FormClosing
        ' If close was selected while running the loopback, shut it down.
        If (bRunning) Then
            StartBtn_Click(Me, Nothing)
        End If
        If (usbDevices.Count) Then
            usbDevices.Dispose()
        End If
    End Sub

    Private Sub NumBytesIn_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumBytesIn.TextChanged

    End Sub
    Private Sub NumBytesOut_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NumBytesOut.TextChanged

    End Sub

    Private Sub break()
        Throw New NotImplementedException
    End Sub


    Private Sub Timer1_Tick(ByVal sender As Object, ByVal e As System.EventArgs) Handles Timer1.Tick

        If TimeCounter >= 50000 Then
            ' Timer1.Enabled = False
            TimeCounter = 0
        Else
            TimeCounter = TimeCounter + 1
            'RxRateTb.Text = "Procedures Run: " & TimeCounter.ToString

            RxRateValue = inCount / TimeCounter / 250
            RxRateValue = Math.Round(RxRateValue, 3)
        End If

    End Sub

End Class


<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.label6 = New System.Windows.Forms.Label()
        Me.ValBox = New System.Windows.Forms.TextBox()
        Me.groupBox1 = New System.Windows.Forms.GroupBox()
        Me.StatusLabel = New System.Windows.Forms.Label()
        Me.groupBox2 = New System.Windows.Forms.GroupBox()
        Me.EPLabel = New System.Windows.Forms.Label()
        Me.StartBtn = New System.Windows.Forms.Button()
        Me.label3 = New System.Windows.Forms.Label()
        Me.label2 = New System.Windows.Forms.Label()
        Me.NumBytesIn = New System.Windows.Forms.TextBox()
        Me.NumBytesOut = New System.Windows.Forms.TextBox()
        Me.label5 = New System.Windows.Forms.Label()
        Me.label4 = New System.Windows.Forms.Label()
        Me.InDataBox = New System.Windows.Forms.TextBox()
        Me.OutdataBox = New System.Windows.Forms.TextBox()
        Me.DeviceTreeView = New System.Windows.Forms.TreeView()
        Me.NumTransfersPassed = New System.Windows.Forms.TextBox()
        Me.label7 = New System.Windows.Forms.Label()
        Me.label1 = New System.Windows.Forms.Label()
        Me.NumTransfersFailed = New System.Windows.Forms.TextBox()
        Me.TxBt = New System.Windows.Forms.Button()
        Me.TranClrBt = New System.Windows.Forms.Button()
        Me.ConnectBT = New System.Windows.Forms.Button()
        Me.TextBoxTx1 = New System.Windows.Forms.TextBox()
        Me.TextBoxRx1 = New System.Windows.Forms.TextBox()
        Me.TxNumTb1 = New System.Windows.Forms.TextBox()
        Me.ClearRxBt = New System.Windows.Forms.Button()
        Me.RxNumTb1 = New System.Windows.Forms.TextBox()
        Me.ClearTxBt = New System.Windows.Forms.Button()
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.ToolStripMenuItem1 = New System.Windows.Forms.ToolStripMenuItem()
        Me.SaveToolStripMenuItemToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.RECONNECTToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.RESETToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.TextBoxTxCmd = New System.Windows.Forms.TextBox()
        Me.TxCmdClrCMD = New System.Windows.Forms.Button()
        Me.TxCmdCopyCMD = New System.Windows.Forms.Button()
        Me.FileTextbox = New System.Windows.Forms.TextBox()
        Me.FileUpCmd = New System.Windows.Forms.Button()
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.RxRateTb = New System.Windows.Forms.TextBox()
        Me.RxTimerTb = New System.Windows.Forms.TextBox()
        Me.Label8 = New System.Windows.Forms.Label()
        Me.Label9 = New System.Windows.Forms.Label()
        Me.Label10 = New System.Windows.Forms.Label()
        Me.groupBox1.SuspendLayout()
        Me.groupBox2.SuspendLayout()
        Me.MenuStrip1.SuspendLayout()
        Me.SuspendLayout()
        '
        'label6
        '
        Me.label6.AutoSize = True
        Me.label6.Location = New System.Drawing.Point(262, 83)
        Me.label6.Name = "label6"
        Me.label6.Size = New System.Drawing.Size(85, 13)
        Me.label6.TabIndex = 43
        Me.label6.Text = "Data Byte(Hex) :"
        '
        'ValBox
        '
        Me.ValBox.Location = New System.Drawing.Point(353, 76)
        Me.ValBox.MaxLength = 8
        Me.ValBox.Name = "ValBox"
        Me.ValBox.Size = New System.Drawing.Size(85, 20)
        Me.ValBox.TabIndex = 42
        Me.ValBox.Text = "12345678"
        '
        'groupBox1
        '
        Me.groupBox1.Controls.Add(Me.StatusLabel)
        Me.groupBox1.Location = New System.Drawing.Point(594, 10)
        Me.groupBox1.Name = "groupBox1"
        Me.groupBox1.Size = New System.Drawing.Size(287, 46)
        Me.groupBox1.TabIndex = 40
        Me.groupBox1.TabStop = False
        Me.groupBox1.Text = "Status"
        '
        'StatusLabel
        '
        Me.StatusLabel.AutoSize = True
        Me.StatusLabel.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.StatusLabel.Location = New System.Drawing.Point(53, 16)
        Me.StatusLabel.Name = "StatusLabel"
        Me.StatusLabel.Size = New System.Drawing.Size(87, 13)
        Me.StatusLabel.TabIndex = 1
        Me.StatusLabel.Text = "Device Status"
        '
        'groupBox2
        '
        Me.groupBox2.Controls.Add(Me.EPLabel)
        Me.groupBox2.Location = New System.Drawing.Point(284, 10)
        Me.groupBox2.Name = "groupBox2"
        Me.groupBox2.Size = New System.Drawing.Size(276, 45)
        Me.groupBox2.TabIndex = 41
        Me.groupBox2.TabStop = False
        Me.groupBox2.Text = "EndPoint Pair OUT/IN"
        '
        'EPLabel
        '
        Me.EPLabel.AutoSize = True
        Me.EPLabel.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.0!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.EPLabel.Location = New System.Drawing.Point(85, 17)
        Me.EPLabel.Name = "EPLabel"
        Me.EPLabel.Size = New System.Drawing.Size(117, 15)
        Me.EPLabel.TabIndex = 0
        Me.EPLabel.Text = "EP2 OUT/ EP6 IN"
        '
        'StartBtn
        '
        Me.StartBtn.Location = New System.Drawing.Point(752, 96)
        Me.StartBtn.Name = "StartBtn"
        Me.StartBtn.Size = New System.Drawing.Size(77, 29)
        Me.StartBtn.TabIndex = 39
        Me.StartBtn.Text = "START"
        Me.StartBtn.UseVisualStyleBackColor = True
        '
        'label3
        '
        Me.label3.AutoSize = True
        Me.label3.Location = New System.Drawing.Point(760, 265)
        Me.label3.Name = "label3"
        Me.label3.Size = New System.Drawing.Size(47, 13)
        Me.label3.TabIndex = 38
        Me.label3.Text = "Bytes IN"
        '
        'label2
        '
        Me.label2.AutoSize = True
        Me.label2.Location = New System.Drawing.Point(411, 263)
        Me.label2.Name = "label2"
        Me.label2.Size = New System.Drawing.Size(59, 13)
        Me.label2.TabIndex = 37
        Me.label2.Text = "Bytes OUT"
        '
        'NumBytesIn
        '
        Me.NumBytesIn.BackColor = System.Drawing.SystemColors.Info
        Me.NumBytesIn.Location = New System.Drawing.Point(823, 261)
        Me.NumBytesIn.Name = "NumBytesIn"
        Me.NumBytesIn.ReadOnly = True
        Me.NumBytesIn.Size = New System.Drawing.Size(85, 20)
        Me.NumBytesIn.TabIndex = 36
        '
        'NumBytesOut
        '
        Me.NumBytesOut.BackColor = System.Drawing.SystemColors.Info
        Me.NumBytesOut.Location = New System.Drawing.Point(484, 260)
        Me.NumBytesOut.Name = "NumBytesOut"
        Me.NumBytesOut.ReadOnly = True
        Me.NumBytesOut.Size = New System.Drawing.Size(85, 20)
        Me.NumBytesOut.TabIndex = 35
        '
        'label5
        '
        Me.label5.AutoSize = True
        Me.label5.Location = New System.Drawing.Point(595, 104)
        Me.label5.Name = "label5"
        Me.label5.Size = New System.Drawing.Size(104, 13)
        Me.label5.TabIndex = 34
        Me.label5.Text = "Bytes Transferred IN"
        '
        'label4
        '
        Me.label4.AutoSize = True
        Me.label4.Location = New System.Drawing.Point(261, 104)
        Me.label4.Name = "label4"
        Me.label4.Size = New System.Drawing.Size(116, 13)
        Me.label4.TabIndex = 33
        Me.label4.Text = "Bytes Transferred OUT"
        '
        'InDataBox
        '
        Me.InDataBox.BackColor = System.Drawing.SystemColors.Window
        Me.InDataBox.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.InDataBox.Location = New System.Drawing.Point(594, 131)
        Me.InDataBox.Multiline = True
        Me.InDataBox.Name = "InDataBox"
        Me.InDataBox.ReadOnly = True
        Me.InDataBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.InDataBox.Size = New System.Drawing.Size(328, 124)
        Me.InDataBox.TabIndex = 32
        '
        'OutdataBox
        '
        Me.OutdataBox.BackColor = System.Drawing.SystemColors.Window
        Me.OutdataBox.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.OutdataBox.Location = New System.Drawing.Point(262, 131)
        Me.OutdataBox.Multiline = True
        Me.OutdataBox.Name = "OutdataBox"
        Me.OutdataBox.ReadOnly = True
        Me.OutdataBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.OutdataBox.Size = New System.Drawing.Size(326, 124)
        Me.OutdataBox.TabIndex = 31
        '
        'DeviceTreeView
        '
        Me.DeviceTreeView.Location = New System.Drawing.Point(12, 66)
        Me.DeviceTreeView.Name = "DeviceTreeView"
        Me.DeviceTreeView.Size = New System.Drawing.Size(244, 189)
        Me.DeviceTreeView.TabIndex = 30
        '
        'NumTransfersPassed
        '
        Me.NumTransfersPassed.BackColor = System.Drawing.SystemColors.Info
        Me.NumTransfersPassed.Location = New System.Drawing.Point(484, 288)
        Me.NumTransfersPassed.Name = "NumTransfersPassed"
        Me.NumTransfersPassed.ReadOnly = True
        Me.NumTransfersPassed.Size = New System.Drawing.Size(85, 20)
        Me.NumTransfersPassed.TabIndex = 65
        '
        'label7
        '
        Me.label7.AutoSize = True
        Me.label7.Location = New System.Drawing.Point(374, 292)
        Me.label7.Name = "label7"
        Me.label7.Size = New System.Drawing.Size(97, 13)
        Me.label7.TabIndex = 64
        Me.label7.Text = "Transfers PASSED"
        '
        'label1
        '
        Me.label1.AutoSize = True
        Me.label1.Location = New System.Drawing.Point(718, 293)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(91, 13)
        Me.label1.TabIndex = 63
        Me.label1.Text = "Transfers FAILED"
        '
        'NumTransfersFailed
        '
        Me.NumTransfersFailed.BackColor = System.Drawing.SystemColors.Info
        Me.NumTransfersFailed.Location = New System.Drawing.Point(823, 289)
        Me.NumTransfersFailed.Name = "NumTransfersFailed"
        Me.NumTransfersFailed.ReadOnly = True
        Me.NumTransfersFailed.Size = New System.Drawing.Size(85, 20)
        Me.NumTransfersFailed.TabIndex = 62
        '
        'TxBt
        '
        Me.TxBt.Location = New System.Drawing.Point(515, 586)
        Me.TxBt.Name = "TxBt"
        Me.TxBt.Size = New System.Drawing.Size(77, 29)
        Me.TxBt.TabIndex = 66
        Me.TxBt.Text = "TX"
        Me.TxBt.UseVisualStyleBackColor = True
        '
        'TranClrBt
        '
        Me.TranClrBt.Location = New System.Drawing.Point(845, 96)
        Me.TranClrBt.Name = "TranClrBt"
        Me.TranClrBt.Size = New System.Drawing.Size(77, 29)
        Me.TranClrBt.TabIndex = 67
        Me.TranClrBt.Text = "CLEAR"
        Me.TranClrBt.UseVisualStyleBackColor = True
        '
        'ConnectBT
        '
        Me.ConnectBT.Location = New System.Drawing.Point(56, 560)
        Me.ConnectBT.Name = "ConnectBT"
        Me.ConnectBT.Size = New System.Drawing.Size(70, 63)
        Me.ConnectBT.TabIndex = 68
        Me.ConnectBT.Text = "CONNECT"
        Me.ConnectBT.UseVisualStyleBackColor = True
        '
        'TextBoxTx1
        '
        Me.TextBoxTx1.BackColor = System.Drawing.SystemColors.Window
        Me.TextBoxTx1.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TextBoxTx1.Location = New System.Drawing.Point(337, 369)
        Me.TextBoxTx1.Multiline = True
        Me.TextBoxTx1.Name = "TextBoxTx1"
        Me.TextBoxTx1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.TextBoxTx1.Size = New System.Drawing.Size(257, 185)
        Me.TextBoxTx1.TabIndex = 69
        Me.TextBoxTx1.Text = "12345678"
        '
        'TextBoxRx1
        '
        Me.TextBoxRx1.BackColor = System.Drawing.SystemColors.Window
        Me.TextBoxRx1.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TextBoxRx1.Location = New System.Drawing.Point(651, 369)
        Me.TextBoxRx1.Multiline = True
        Me.TextBoxRx1.Name = "TextBoxRx1"
        Me.TextBoxRx1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.TextBoxRx1.Size = New System.Drawing.Size(257, 185)
        Me.TextBoxRx1.TabIndex = 70
        Me.TextBoxRx1.Text = "RX"
        '
        'TxNumTb1
        '
        Me.TxNumTb1.BackColor = System.Drawing.SystemColors.Window
        Me.TxNumTb1.Location = New System.Drawing.Point(515, 560)
        Me.TxNumTb1.Name = "TxNumTb1"
        Me.TxNumTb1.ReadOnly = True
        Me.TxNumTb1.Size = New System.Drawing.Size(77, 20)
        Me.TxNumTb1.TabIndex = 74
        '
        'ClearRxBt
        '
        Me.ClearRxBt.Location = New System.Drawing.Point(831, 586)
        Me.ClearRxBt.Name = "ClearRxBt"
        Me.ClearRxBt.Size = New System.Drawing.Size(77, 29)
        Me.ClearRxBt.TabIndex = 75
        Me.ClearRxBt.Text = "CLEAR"
        Me.ClearRxBt.UseVisualStyleBackColor = True
        '
        'RxNumTb1
        '
        Me.RxNumTb1.BackColor = System.Drawing.SystemColors.Window
        Me.RxNumTb1.Location = New System.Drawing.Point(831, 560)
        Me.RxNumTb1.Name = "RxNumTb1"
        Me.RxNumTb1.ReadOnly = True
        Me.RxNumTb1.Size = New System.Drawing.Size(77, 20)
        Me.RxNumTb1.TabIndex = 76
        '
        'ClearTxBt
        '
        Me.ClearTxBt.Location = New System.Drawing.Point(432, 586)
        Me.ClearTxBt.Name = "ClearTxBt"
        Me.ClearTxBt.Size = New System.Drawing.Size(77, 29)
        Me.ClearTxBt.TabIndex = 77
        Me.ClearTxBt.Text = "CLEAR"
        Me.ClearTxBt.UseVisualStyleBackColor = True
        '
        'MenuStrip1
        '
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.ToolStripMenuItem1, Me.SaveToolStripMenuItemToolStripMenuItem, Me.RECONNECTToolStripMenuItem, Me.RESETToolStripMenuItem})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Size = New System.Drawing.Size(952, 24)
        Me.MenuStrip1.TabIndex = 78
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'ToolStripMenuItem1
        '
        Me.ToolStripMenuItem1.Name = "ToolStripMenuItem1"
        Me.ToolStripMenuItem1.Size = New System.Drawing.Size(50, 20)
        Me.ToolStripMenuItem1.Text = "LOAD"
        '
        'SaveToolStripMenuItemToolStripMenuItem
        '
        Me.SaveToolStripMenuItemToolStripMenuItem.Name = "SaveToolStripMenuItemToolStripMenuItem"
        Me.SaveToolStripMenuItemToolStripMenuItem.Size = New System.Drawing.Size(46, 20)
        Me.SaveToolStripMenuItemToolStripMenuItem.Text = "SAVE"
        '
        'RECONNECTToolStripMenuItem
        '
        Me.RECONNECTToolStripMenuItem.Name = "RECONNECTToolStripMenuItem"
        Me.RECONNECTToolStripMenuItem.Size = New System.Drawing.Size(88, 20)
        Me.RECONNECTToolStripMenuItem.Text = "RECONNECT"
        '
        'RESETToolStripMenuItem
        '
        Me.RESETToolStripMenuItem.Name = "RESETToolStripMenuItem"
        Me.RESETToolStripMenuItem.Size = New System.Drawing.Size(51, 20)
        Me.RESETToolStripMenuItem.Text = "RESET"
        '
        'TextBoxTxCmd
        '
        Me.TextBoxTxCmd.BackColor = System.Drawing.SystemColors.Window
        Me.TextBoxTxCmd.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.TextBoxTxCmd.Location = New System.Drawing.Point(29, 369)
        Me.TextBoxTxCmd.Multiline = True
        Me.TextBoxTxCmd.Name = "TextBoxTxCmd"
        Me.TextBoxTxCmd.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.TextBoxTxCmd.Size = New System.Drawing.Size(257, 185)
        Me.TextBoxTxCmd.TabIndex = 79
        Me.TextBoxTxCmd.Text = "CMD"
        '
        'TxCmdClrCMD
        '
        Me.TxCmdClrCMD.Location = New System.Drawing.Point(188, 559)
        Me.TxCmdClrCMD.Name = "TxCmdClrCMD"
        Me.TxCmdClrCMD.Size = New System.Drawing.Size(84, 31)
        Me.TxCmdClrCMD.TabIndex = 83
        Me.TxCmdClrCMD.Text = "CMD CLEAR"
        Me.TxCmdClrCMD.UseVisualStyleBackColor = True
        '
        'TxCmdCopyCMD
        '
        Me.TxCmdCopyCMD.Location = New System.Drawing.Point(188, 592)
        Me.TxCmdCopyCMD.Name = "TxCmdCopyCMD"
        Me.TxCmdCopyCMD.Size = New System.Drawing.Size(84, 31)
        Me.TxCmdCopyCMD.TabIndex = 82
        Me.TxCmdCopyCMD.Text = "CMD LOAD"
        Me.TxCmdCopyCMD.UseVisualStyleBackColor = True
        '
        'FileTextbox
        '
        Me.FileTextbox.BackColor = System.Drawing.SystemColors.Info
        Me.FileTextbox.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.FileTextbox.Location = New System.Drawing.Point(29, 309)
        Me.FileTextbox.Multiline = True
        Me.FileTextbox.Name = "FileTextbox"
        Me.FileTextbox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical
        Me.FileTextbox.Size = New System.Drawing.Size(174, 22)
        Me.FileTextbox.TabIndex = 88
        '
        'FileUpCmd
        '
        Me.FileUpCmd.Location = New System.Drawing.Point(209, 304)
        Me.FileUpCmd.Name = "FileUpCmd"
        Me.FileUpCmd.Size = New System.Drawing.Size(77, 29)
        Me.FileUpCmd.TabIndex = 87
        Me.FileUpCmd.Text = "UPDATE"
        Me.FileUpCmd.UseVisualStyleBackColor = True
        '
        'Timer1
        '
        Me.Timer1.Interval = 1000
        '
        'RxRateTb
        '
        Me.RxRateTb.BackColor = System.Drawing.SystemColors.Info
        Me.RxRateTb.Location = New System.Drawing.Point(823, 315)
        Me.RxRateTb.Name = "RxRateTb"
        Me.RxRateTb.ReadOnly = True
        Me.RxRateTb.Size = New System.Drawing.Size(85, 20)
        Me.RxRateTb.TabIndex = 89
        '
        'RxTimerTb
        '
        Me.RxTimerTb.BackColor = System.Drawing.SystemColors.Info
        Me.RxTimerTb.Location = New System.Drawing.Point(823, 341)
        Me.RxTimerTb.Name = "RxTimerTb"
        Me.RxTimerTb.ReadOnly = True
        Me.RxTimerTb.Size = New System.Drawing.Size(85, 20)
        Me.RxTimerTb.TabIndex = 90
        '
        'Label8
        '
        Me.Label8.AutoSize = True
        Me.Label8.Location = New System.Drawing.Point(739, 319)
        Me.Label8.Name = "Label8"
        Me.Label8.Size = New System.Drawing.Size(78, 13)
        Me.Label8.TabIndex = 91
        Me.Label8.Text = "Rx Rate(KB/S)"
        '
        'Label9
        '
        Me.Label9.AutoSize = True
        Me.Label9.Location = New System.Drawing.Point(721, 344)
        Me.Label9.Name = "Label9"
        Me.Label9.Size = New System.Drawing.Size(96, 13)
        Me.Label9.TabIndex = 92
        Me.Label9.Text = "Elapsed Time(Sec)"
        '
        'Label10
        '
        Me.Label10.AutoSize = True
        Me.Label10.Location = New System.Drawing.Point(32, 294)
        Me.Label10.Name = "Label10"
        Me.Label10.Size = New System.Drawing.Size(67, 13)
        Me.Label10.TabIndex = 93
        Me.Label10.Text = "Config Script"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(952, 639)
        Me.Controls.Add(Me.Label10)
        Me.Controls.Add(Me.Label9)
        Me.Controls.Add(Me.Label8)
        Me.Controls.Add(Me.RxTimerTb)
        Me.Controls.Add(Me.RxRateTb)
        Me.Controls.Add(Me.FileTextbox)
        Me.Controls.Add(Me.FileUpCmd)
        Me.Controls.Add(Me.TxCmdClrCMD)
        Me.Controls.Add(Me.TxCmdCopyCMD)
        Me.Controls.Add(Me.TextBoxTxCmd)
        Me.Controls.Add(Me.ClearTxBt)
        Me.Controls.Add(Me.RxNumTb1)
        Me.Controls.Add(Me.ClearRxBt)
        Me.Controls.Add(Me.TxNumTb1)
        Me.Controls.Add(Me.TextBoxRx1)
        Me.Controls.Add(Me.TextBoxTx1)
        Me.Controls.Add(Me.ConnectBT)
        Me.Controls.Add(Me.TranClrBt)
        Me.Controls.Add(Me.TxBt)
        Me.Controls.Add(Me.NumTransfersPassed)
        Me.Controls.Add(Me.label7)
        Me.Controls.Add(Me.label1)
        Me.Controls.Add(Me.NumTransfersFailed)
        Me.Controls.Add(Me.label6)
        Me.Controls.Add(Me.ValBox)
        Me.Controls.Add(Me.groupBox1)
        Me.Controls.Add(Me.groupBox2)
        Me.Controls.Add(Me.StartBtn)
        Me.Controls.Add(Me.label3)
        Me.Controls.Add(Me.label2)
        Me.Controls.Add(Me.NumBytesIn)
        Me.Controls.Add(Me.NumBytesOut)
        Me.Controls.Add(Me.label5)
        Me.Controls.Add(Me.label4)
        Me.Controls.Add(Me.InDataBox)
        Me.Controls.Add(Me.OutdataBox)
        Me.Controls.Add(Me.DeviceTreeView)
        Me.Controls.Add(Me.MenuStrip1)
        Me.MainMenuStrip = Me.MenuStrip1
        Me.Name = "Form1"
        Me.Text = "USB COMMANDER"
        Me.groupBox1.ResumeLayout(False)
        Me.groupBox1.PerformLayout()
        Me.groupBox2.ResumeLayout(False)
        Me.groupBox2.PerformLayout()
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Private WithEvents label6 As System.Windows.Forms.Label
    Private WithEvents ValBox As System.Windows.Forms.TextBox
    Private WithEvents groupBox1 As System.Windows.Forms.GroupBox
    Private WithEvents StatusLabel As System.Windows.Forms.Label
    Private WithEvents groupBox2 As System.Windows.Forms.GroupBox
    Private WithEvents EPLabel As System.Windows.Forms.Label
    Private WithEvents StartBtn As System.Windows.Forms.Button
    Private WithEvents label3 As System.Windows.Forms.Label
    Private WithEvents label2 As System.Windows.Forms.Label
    Private WithEvents NumBytesIn As System.Windows.Forms.TextBox
    Private WithEvents NumBytesOut As System.Windows.Forms.TextBox
    Private WithEvents label5 As System.Windows.Forms.Label
    Private WithEvents label4 As System.Windows.Forms.Label
    Private WithEvents InDataBox As System.Windows.Forms.TextBox
    Private WithEvents OutdataBox As System.Windows.Forms.TextBox
    Friend WithEvents DeviceTreeView As System.Windows.Forms.TreeView
    Private WithEvents NumTransfersPassed As System.Windows.Forms.TextBox
    Private WithEvents label7 As System.Windows.Forms.Label
    Private WithEvents label1 As System.Windows.Forms.Label
    Private WithEvents NumTransfersFailed As System.Windows.Forms.TextBox
    Private WithEvents TxBt As Button
    Private WithEvents TranClrBt As Button
    Private WithEvents ConnectBT As Button
    Private WithEvents TextBoxTx1 As TextBox
    Private WithEvents TextBoxRx1 As TextBox
    Private WithEvents TxNumTb1 As TextBox
    Private WithEvents ClearRxBt As Button
    Private WithEvents RxNumTb1 As TextBox
    Private WithEvents ClearTxBt As Button
    Friend WithEvents MenuStrip1 As MenuStrip
    Friend WithEvents SaveToolStripMenuItemToolStripMenuItem As ToolStripMenuItem
    Private WithEvents TextBoxTxCmd As TextBox
    Private WithEvents TxCmdClrCMD As Button
    Private WithEvents TxCmdCopyCMD As Button
    Private WithEvents FileTextbox As TextBox
    Private WithEvents FileUpCmd As Button
    Friend WithEvents ToolStripMenuItem1 As ToolStripMenuItem
    Friend WithEvents RECONNECTToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents RESETToolStripMenuItem As ToolStripMenuItem
    Friend WithEvents Timer1 As Timer
    Private WithEvents RxRateTb As TextBox
    Private WithEvents RxTimerTb As TextBox
    Private WithEvents Label8 As Label
    Private WithEvents Label9 As Label
    Private WithEvents Label10 As Label
End Class

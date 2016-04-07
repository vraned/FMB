using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using CyUSB;

namespace Bulkloop_VCS
{
    public partial class Form1 : Form
    {
        const int VID = 0x04B4;
        const int PID = 0x00F0;

        CyUSBDevice myDevice = null;                                                    // myDevice represents the bulkloop device in this example
        USBDeviceList usbDevices = null;                                                // Dynamic list of USBdevcies bound to CyUSB.sys
        CyBulkEndPoint BulkInEndPt = null;                                              // We need one Bulk IN endpoint
        CyBulkEndPoint BulkOutEndPt = null;                                             // We need one Bulk OUT endpoint

        Thread tXfers;                                                                  // Thread where data transfers are handled
        bool bRunning = false;                                                          // Variable determining whether tXfers thread is running or not
        int value, value1, value2, value3;
        int byteCount = 0;
        long outCount, inCount;                                                         // Number of bytes transferred in out and in direction 
        long passCount, failCount;														// Number of transfers PASSED and FAILED
        int XFERSIZE = 1024;                                                       // Transfers happen in chunks of 512 bytes in case of High speed; and 1024 bytes in case of superspeed
        byte[] outData = new byte[1024];                                            // OUT transfers send data in the outData buffer
        byte[] inData = new byte[1024];                                             // IN transfers send data into the inData buffer

        // Setup the callback routine for updating the UI
        delegate void UpdateUICallback();
        UpdateUICallback updateUI;
 
        public Form1()
        {
            InitializeComponent();

            usbDevices = new USBDeviceList(CyConst.DEVICES_CYUSB);                       // Dynamic list of USBdevcies bound to CyUSB.sys
            
            //Adding event handlers for device attachment and device removal
            usbDevices.DeviceAttached += new EventHandler(usbDevices_DeviceAttached);   
            usbDevices.DeviceRemoved += new EventHandler(usbDevices_DeviceRemoved);                 
            RefreshDevice();

            // Setup the callback routine for updating the UI
            updateUI = new UpdateUICallback(StatusUpdate);                               
            tXfers = new Thread(new ThreadStart(TransfersThread));
            tXfers.IsBackground = true;
            tXfers.Priority = ThreadPriority.Highest;            
        }

        /* Summary
           Event handler for device attachment event
      */
        public void usbDevices_DeviceAttached(object sender, EventArgs e)               // Eventhandler assigned to DeviceAttached
        {
            RefreshDevice();                                                                       
        }

        /* Summary
           Event handler for device removal event
        */
        public void usbDevices_DeviceRemoved(object sender, EventArgs e)                 // Eventhandler assigned to DeviceRemoved
        {
            RefreshDevice();                                                              
        }

        /* Summary
            The function gets the device with VID=04b4 and PID=00F0; updates the Status label and Treeview
        */
        public void RefreshDevice()
        {

            myDevice = usbDevices[VID, PID] as CyUSBDevice;                        // Instantiating myDevice with handle to the device of interest (bulkloop device in this example)

            if (myDevice == null)                                                        // Bulkloop device if not found
            {
                StatusLabel.Text = "No Bulkloop Device Detected";
                StartBtn.Enabled = false;
                StartBtn.Text = "Start";
                StartBtn.BackColor = Color.Aquamarine;
            }
            else                                                                        // Bulkloop device if found
            {
                StatusLabel.Text = "Bulkloop Device connected";
                StartBtn.Enabled = true;
                BulkOutEndPt = myDevice.EndPointOf(0x02) as CyBulkEndPoint;             // Instantiating the endpoints
                BulkInEndPt = myDevice.EndPointOf(0x86) as CyBulkEndPoint;

            }

            // Clear and update TreeView
            DeviceTreeView.Nodes.Clear();
            foreach (CyUSBDevice dev in usbDevices)
                DeviceTreeView.Nodes.Add(dev.Tree);
        }

        /* Summary
           Event handler for Click event of Start/ Stop button. Starts or stops the thread and accordingly updates the text of the StartBtn.
        */
        private void StartBtn_Click(object sender, EventArgs e)
        {
            if ((BulkOutEndPt == null) && (BulkInEndPt == null))                                         // check for NULL endpoints
            {
                MessageBox.Show("No EP2 BULK OUT and EP6 BULK IN endpoints");
                StartBtn.Text = "Start";
                StartBtn.BackColor = Color.Aquamarine;
                goto my_label;
            }
            else
            {
                if (BulkOutEndPt == null) 
                {
                    MessageBox.Show("No EP2 BULK OUT endpoint");
                    StartBtn.Text = "Start";
                    StartBtn.BackColor = Color.Aquamarine;
                    goto my_label;
                }
                if (BulkInEndPt == null)
                {
                    MessageBox.Show("No EP6 BULK IN endpoint");
                    StartBtn.Text = "Start";
                    StartBtn.BackColor = Color.Aquamarine;
                    goto my_label;
                }
            }

            if (!bRunning)
            {
                if (ValBox.Text != "")
                {
                    byte[] byteArray = Encoding.ASCII.GetBytes(ValBox.Text);
                    byteCount = Encoding.ASCII.GetByteCount(ValBox.Text);
                    value = 0;
                    value1 = 0;
                    value2 = 0;
                    value3 = 0;

                    if (byteCount >= 1) value = Convert.ToInt32(Encoding.ASCII.GetString(byteArray, 0, 1), 16);
                    if (byteCount >= 2) value = Convert.ToInt32(Encoding.ASCII.GetString(byteArray, 0, 2), 16);
                    if (byteCount >= 3) value1 = Convert.ToInt32(Encoding.ASCII.GetString(byteArray, 2, 1), 16);
                    if (byteCount >= 4) value1 = Convert.ToInt32(Encoding.ASCII.GetString(byteArray, 2, 2), 16);
                    if (byteCount >= 5) value2 = Convert.ToInt32(Encoding.ASCII.GetString(byteArray, 4, 1), 16);
                    if (byteCount >= 6) value2 = Convert.ToInt32(Encoding.ASCII.GetString(byteArray, 4, 2), 16);
                    if (byteCount >= 7) value3 = Convert.ToInt32(Encoding.ASCII.GetString(byteArray, 6, 1), 16);
                    if (byteCount >= 8) value3 = Convert.ToInt32(Encoding.ASCII.GetString(byteArray, 6, 2), 16);
                }
                outCount = 0;
                inCount = 0;
                failCount = 0;
                passCount = 0;

                OutdataBox.Text = "";
                InDataBox.Text = "";

                bRunning = true;
                StartBtn.Text = "Stop";
                StartBtn.BackColor = Color.Pink;

                //creates new thread
                tXfers = new Thread(new ThreadStart(TransfersThread));
                tXfers.IsBackground = true;
                tXfers.Priority = ThreadPriority.Highest;
                //Starts the new thread
                tXfers.Start();
            }
            else
            {
                //Makes the thread stop and aborts the thread
                bRunning = false;
                StartBtn.Text = "Start";
                StartBtn.BackColor = Color.Aquamarine;

                if (tXfers == null) return;

                if (tXfers.IsAlive)
                {
                    tXfers.Abort();
                    tXfers.Join();
                    tXfers = null;
                    BulkOutEndPt.Reset();
                }
            }
        my_label:
            ;
        }

        /* Summary
          This thread is initiated on start button click. Runs the thread; invokes the data transfers and invokes the StatusUpdate to update the UI 
       */

        public void TransfersThread()
        {
            int xferLen = XFERSIZE;

            bool bResult = true;
            if (myDevice.bSuperSpeed)
                XFERSIZE = 1024;
            else if (myDevice.bHighSpeed)
                XFERSIZE = 512;
            // Loop stops if either an IN or OUT transfer fails
            for (; bRunning && bResult; )
            {
                SetOutputData();                                                         // This function sets the bytes in outData to be transferred to OUT EP
                xferLen = XFERSIZE;
                // Calls the XferData function for bulk transfer(OUT) in the cyusb.dll
                bResult = BulkOutEndPt.XferData(ref outData, ref xferLen);
                outCount += xferLen;

                if (bResult)
                {
                    // Calls the XferData function for bulk transfer(IN) in the cyusb.dll
                    bResult = BulkInEndPt.XferData(ref inData, ref xferLen);
                    inCount += xferLen;
                }

                passCount = passCount + 1; 
                for (int i = 0; i < xferLen - 1; i++)
                {
                    if (inData[i] != outData[i])
                    {
                        failCount = failCount + 1;
                        passCount = passCount - 1;
                        break;
                    }
                }
                              
                // Call StatusUpdate() in the main thread
                this.Invoke(updateUI);
            }
            // Call StatusUpdate() in the main thread
            this.Invoke(updateUI);
            my_label1:
            bRunning = false;

            
        }
        /* Summary
           Takes the user-given data in the ‘Data Bytes(Hex)’ field of the application user interface and fills the ‘outData[]’ buffer with these bytes 
        */
        private void SetOutputData()
        {
            for (int i = 0; i < XFERSIZE; i++)
            {
                if ((byteCount >= 1) && (i < XFERSIZE)) outData[i] = (byte)value;
                if ((byteCount >= 3) && (++i < XFERSIZE)) outData[i] = (byte)value1;
                if ((byteCount >= 5) && (++i < XFERSIZE)) outData[i] = (byte)value2;
                if ((byteCount >= 7) && (++i < XFERSIZE)) outData[i] = (byte)value3;
            }

        }


        /* Summary
           This is the call back function for updating the UI(user interface) and is called from TransfersThread. This updates the OutdataBox,InDataBox
           with the Bytes transferred; and NumBytesOut,NumBytesIn with the number of Bytes transferred.
         
       */
        public void StatusUpdate()
        {
            StringBuilder dataStrOut = new StringBuilder();
            StringBuilder dataStrIn = new StringBuilder();

            if (bRunning)
            {
                for (int i = 0; i < XFERSIZE; i++)
                {
                    if ((i % 16) == 0) dataStrOut.Append(string.Format("\r\n{0:X4}", i));
                    dataStrOut.Append(string.Format(" {0:X2}", outData[i]));
                }
                dataStrOut.Append(string.Format("\r\n"));
                OutdataBox.Text += dataStrOut.ToString();                                   //This updates the OutdataBox with bytes transferred OUT

                for (int i = 0; i < XFERSIZE; i++)
                {
                    if ((i % 16) == 0) dataStrIn.Append(string.Format("\r\n{0:X4}", i));
                    dataStrIn.Append(string.Format(" {0:X2}", inData[i]));
                }
                dataStrIn.Append(string.Format("\r\n"));
                InDataBox.Text += dataStrIn.ToString();                                      //This updates the InDataBox with bytes transferred IN
            }


            NumBytesOut.Text = outCount.ToString();                                          //This updates the NumBytesOut with number of bytes transferred OUT
            NumBytesIn.Text = inCount.ToString();                                           //This updates the NumBytesIn with number of bytes transferred IN
            NumTransfersFailed.Text = failCount.ToString();                                 //This updates the NumTransfersFailed with number of transfers FAILED
            NumTransfersPassed.Text = passCount.ToString();                                 //This updates the NumTransfersPassed with number of transfers PASSED
            Refresh();

            StartBtn.Text = bRunning ? "Stop" : "Start";
            StartBtn.BackColor = bRunning ? Color.Pink : Color.Aquamarine;

        }
        /* Summary
           Eventhandler for Form1 Close event
        */
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            // If close was selected while running the loopback, shut it down.
            if (bRunning)
                StartBtn_Click(this, null);

            if (usbDevices != null) usbDevices.Dispose();
        }
                              
    }
}

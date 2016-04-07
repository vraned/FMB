using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using System.IO.Ports;
using System.Windows.Forms;
using Test_Fluidics;
using System.Collections;

namespace Test_Fluidics
{
    public partial class Form1 : Form
    {
        

        // Mode Table Filename
        static string filename = "CytovilleModeTable.xml";

        // USB Virtual Serial Port
        static SerialPort sPort = new SerialPort();

        // Mode Table
        ModeTable cfmt;

        // Comms Listener
        private Thread readThread;

        // Variable Monitor
        private Thread monitorThread;

        // Serial Port Mutex
        private static Mutex serMut = new Mutex();

        private static bool monitorPoll = false;
        private static bool sensirionPoll = false;

        // Command
        private byte[] globmsg = new byte[1];
        private bool takes_args;

        delegate void SetTextCallback(string text, TextBox textBox);
        delegate void ChangeButtonColorCallback(Color color, Button button);

        public Form1()
        {
            InitializeComponent();

            // Initialize Commands
            ArrayList Commands = new ArrayList();

            Commands.Add(new Command("eStop", Command.E_STOP, false));
            Commands.Add(new Command("setPress", Command.SET_PRESS, true));
            Commands.Add(new Command("getPress", Command.GET_PRESS, false));
            Commands.Add(new Command("getSample", Command.GET_SAMPLE, false));
            Commands.Add(new Command("setPresTol", Command.SET_PRESS_TOL, true));
            Commands.Add(new Command("getPresTol", Command.GET_PRESS_TOL, false));
            Commands.Add(new Command("setRefTemp", Command.SET_REF_TEMP, true));
            Commands.Add(new Command("getRefTemp", Command.GET_REF_TEMP, false));
            Commands.Add(new Command("getTemp", Command.GET_TEMP, false));
            Commands.Add(new Command("setTempFactor", Command.SET_TEMP_FACTOR, true));
            Commands.Add(new Command("getTempFactor", Command.GET_TEMP_FACTOR, false));
            Commands.Add(new Command("execMode", Command.EXEC_MODE, true));
            Commands.Add(new Command("setCalValues", Command.SET_CAL_VALUES, true));
            Commands.Add(new Command("getCalValues", Command.GET_CAL_VALUES, false));
            Commands.Add(new Command("getSens", Command.GET_SENSOR, true));
            Commands.Add(new Command("getValve", Command.GET_VALVE, true));
            Commands.Add(new Command("getPump", Command.GET_PUMP, true));
            Commands.Add(new Command("setValve", Command.SET_VALVE, true));
            Commands.Add(new Command("setPump", Command.SET_PUMP, true));
            Commands.Add(new Command("getLaser", Command.GET_LASER, true));
            Commands.Add(new Command("setLaser", Command.SET_LASER, true));
            Commands.Add(new Command("setLaserDelay", Command.SET_LASER_DELAY, true));
            Commands.Add(new Command("increaseFlowRate", Command.INCREASE_FLOW_RATE, true));
            Commands.Add(new Command("decreaseFlowRate", Command.DECREASE_FLOW_RATE, true));
            Commands.Add(new Command("ReadSensirion", Command.READ_SENSIRION, false));
            Commands.Add(new Command("setController", Command.SET_CONTROLLER, true)); //zrw
            Commands.Add(new Command("setPID", Command.SET_PID, true)); //zrw
            Commands.Add(new Command("getPID", Command.GET_PID, false)); //zrw
            Commands.Add(new Command("getSITtolerance", Command.GET_SIT_TOL, false)); //zrw
            Commands.Add(new Command("getSITposition", Command.GET_SIT_POS, false)); //zrw
            Commands.Add(new Command("setSITtolerance", Command.SET_SIT_TOL, false)); //zrw
            Commands.Add(new Command("calSIT", Command.CALIBRATE_SIT, false)); //zrw
            Commands.Add(new Command("moveSIT", Command.MOVE_SIT, false)); //zrw

            comboBoxCommand.DataSource = Commands;

            comboBoxCommand.DisplayMember = "Msg";
            //comboBoxCommand.ValueMember = "Code";
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
         
            

            
        }

        private void ReadModeTable(string filename)
        {
            // Deserialize from XML file into Class type
            XmlSerializer serializer = new XmlSerializer(typeof(ModeTable));

            try
            {
                FileStream fs = new FileStream(filename, FileMode.Open);

                // Use the Deserialize method to restore the object's state with
                // data from the XML document. 
                cfmt = (ModeTable)serializer.Deserialize(fs);
                
                fs.Close();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.Message, "FLD Test APP ERROR!!!");
            }
        }

        private void buttonloadmt_Click(object sender, EventArgs e)
        {

            if (sPort.IsOpen != true) return;

            // Load Mode Table from Disk 
            ReadModeTable(filename);

            listBoxModes.Items.Clear();

            if (cfmt != null)
            {
                foreach (CytovilleFluidicsMode mode in cfmt.Mode)
                {
                    // Populate listBox
                    listBoxModes.Items.Add(mode.ModeName);
                }


                // Send Mode Table to FLD 
                serMut.WaitOne();
                byte[] msg = new byte[1];
                msg[0] = Command.LOAD_MODE_TABLE;
                sPort.Write(msg, 0, 1);

                // Timing Table first
                float[] buf1 = new float[20];
                buf1[0]  = cfmt.TimingTable.KT01;
                buf1[1]  = cfmt.TimingTable.KT02;
                buf1[2]  = cfmt.TimingTable.KT03;
                buf1[3]  = cfmt.TimingTable.KT04;
                buf1[4]  = cfmt.TimingTable.KT05;
                buf1[5]  = cfmt.TimingTable.KT06;
                buf1[6]  = cfmt.TimingTable.KT07;
                buf1[7]  = cfmt.TimingTable.KT08;
                buf1[8]  = cfmt.TimingTable.KT09;
                buf1[9]  = cfmt.TimingTable.KT10;
                buf1[10] = cfmt.TimingTable.KT11;
                buf1[11] = cfmt.TimingTable.KT12;
                buf1[12] = cfmt.TimingTable.KT13;
                buf1[13] = cfmt.TimingTable.KT14;
                buf1[14] = cfmt.TimingTable.KT15;
                buf1[15] = cfmt.TimingTable.KT16;
                buf1[16] = cfmt.TimingTable.KT17;
                buf1[17] = cfmt.TimingTable.KT18;
                buf1[18] = cfmt.TimingTable.KT19;
                buf1[19] = cfmt.TimingTable.KT20;

                // create a byte array and copy the floats into it...
                var buffer = new byte[buf1.Length * sizeof(float)];
                Buffer.BlockCopy(buf1, 0, buffer, 0, buffer.Length);
                sPort.Write(buffer, 0, buffer.Length);

                // Calibration Table
                // Pressure DIFF
                buf1[0] = cfmt.CalibrationTable.Pressure.Diff;
                Buffer.BlockCopy(buf1, 0, buffer, 0, sizeof(float));
                sPort.Write(buffer, 0, sizeof(float));

                // SampleFlow
                UInt32[] buf2 = new UInt32[10];
                buf2[0] = (UInt32)cfmt.CalibrationTable.SampleFlow.Low;
                buf2[1] = (UInt32)cfmt.CalibrationTable.SampleFlow.Med;
                buf2[2] = (UInt32)cfmt.CalibrationTable.SampleFlow.High;
                Buffer.BlockCopy(buf2, 0, buffer, 0, 3 * sizeof(UInt32));
                sPort.Write(buffer, 0, 3 * sizeof(UInt32));

                // SIT
                buf2[0] = (UInt32)cfmt.CalibrationTable.SIT.SITU;
                buf2[1] = (UInt32)cfmt.CalibrationTable.SIT.SITD;
                buf2[2] = (UInt32)cfmt.CalibrationTable.SIT.SITP;
                Buffer.BlockCopy(buf2, 0, buffer, 0, 3 * sizeof(UInt32));
                sPort.Write(buffer, 0, 3 * sizeof(UInt32));

                int modenum = 1;

                // Send Down Modes
                foreach (CytovilleFluidicsMode mode in cfmt.Mode)
                {
                    foreach (CytovilleFluidicsModeTimeSegment ts in mode.TimeSegment)
                    {
                        byte[] b = new byte[10];

                        // Send Valves
                        b[0] = (byte)ts.Valve.V1;
                        b[1] = (byte)ts.Valve.V2;
                        b[2] = (byte)ts.Valve.V3;
                        b[3] = (byte)ts.Valve.V4;
                        b[4] = (byte)ts.Valve.V5;
                        b[5] = (byte)ts.Valve.V6;
                        sPort.Write(b, 0, 6);
                        
                        // Send Pumps
                        b[0] = (byte)ts.Pump.VACUUM;
                        b[1] = (byte)ts.Pump.P1;
                        b[2] = (byte)ts.Pump.P2;
                        b[3] = (byte)ts.Pump.P3;
                        sPort.Write(b, 0, 4);

                        // Send Sample Flowrate
                        b[0] = (byte)ts.Sample.FlowRate;
                        sPort.Write(b, 0, 1);

                        // Send Time Last

                        switch (ts.Time)
                        {
                            case "SITU"   : b[0] = 21; break;
                            case "SITD"   : b[0] = 22; break;
                            case "SITP"   : b[0] = 23; break;
                            case "TUBE"   : b[0] = 24; break;
                            case "NO_TUBE": b[0] = 25; break;
                            case "TOGG"   : b[0] = 26; break;

                            default:
                                // It's a KTxx Entry (1 to 20)
                                b[0] = Byte.Parse(ts.Time.Remove(0, 2));
                                break;
                        }

                        if ((b[0] == 26) && (cfmt.Mode.Length == modenum))
                        {
                            // Send an extra termination byte
                            b[1] = 0xAB;
                            sPort.Write(b, 0, 2);
                        }
                        else 
                        {
                            sPort.Write(b, 0, 1);
                        }  
                    }
                    modenum++;
                }
                serMut.ReleaseMutex();
            }
        }

        

        private void listBoxModes_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        public void ReadSerialPort()
        {
            while (true)
            {
                try
                {

                    string message = sPort.ReadLine();

                    char[] delimiterChars = { ' ' };

                    // Some Messages we just ignore
                    //if (message.Contains("setPump:")  ||
                    //    message.Contains("setValve:") ||
                    //    message.Contains("execModeBegin:"))
                    //{
                    //    continue;
                    //}

                    if (message.Contains("getTemp:"))
                    {
                        string[] arguments = message.Split(delimiterChars);
                        SetText(arguments[1], textBoxTemp);
                    }
                    else if (message.Contains("execModeEnd:"))
                    {
                        SetText(message, textBoxFLDMsg);
                        ChangeButtonColor(Color.Black, buttonExecMode);
                    }
                    else if (message.Contains("getPres:"))
                    {
                        string[] arguments = message.Split(delimiterChars);
                        SetText(arguments[1], textBoxVac);
                    }
                    else if (message.Contains("getFlowRate:"))
                    {
                        string[] arguments = message.Split(delimiterChars);
                        SetText(arguments[1], textBoxSetFlowRate);
                    }
                    else if (message.Contains("readSensirion:"))
                    {
                        string[] arguments = message.Split(delimiterChars);
                        SetText(arguments[1], textBoxSensirion);
                    }
                    else if (message.Contains("getSample:"))
                    {
                        string[] arguments = message.Split(delimiterChars);
                        SetText(arguments[1], textBoxSample);
                    }
                    else if (message.Contains("getSensor:"))
                    {
                        string[] arguments = message.Split(delimiterChars);
                        switch (arguments[1])
                        {
                            case "1":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS1);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS1);
                                    }
                                }
                                break;
                            case "2":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS2);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS2);
                                    }
                                }
                                break;
                            case "3":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS3);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS3);
                                    }
                                }
                                break;
                            case "4":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS4);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS4);
                                    }
                                }
                                break;
                            case "5":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS5);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS5);
                                    }
                                }
                                break;
                            case "6":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS6);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS6);
                                    }
                                }
                                break;
                            case "7":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS7);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS7);
                                    }
                                }
                                break;
                            case "8":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS8);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS8);
                                    }
                                }
                                break;
                            case "9":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS9);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS9);
                                    }
                                }
                                break;
                            case "10":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS10);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS10);
                                    }
                                }
                                break;
                            case "11":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS11);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS11);
                                    }
                                }
                                break;
                            case "12":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS12);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS12);
                                    }
                                }
                                break;
                            case "13":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS13);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS13);
                                    }
                                }
                                break;
                            case "14":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS14);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS14);
                                    }
                                }
                                break;
                            case "15":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS15);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS15);
                                    }
                                }
                                break;
                            case "16":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS16);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS16);
                                    }
                                }
                                break;
                            case "17":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS17);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS17);
                                    }
                                }
                                break;
                            case "18":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS18);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS18);
                                    }
                                }
                                break;
                            case "19":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS19);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS19);
                                    }
                                }
                                break;
                            case "20":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxS20);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxS20);
                                    }
                                }
                                break;
                        }
                    }
                    else if (message.Contains("getValve:"))
                    {
                        string[] arguments = message.Split(delimiterChars);
                        switch (arguments[1])
                        {
                            case "1":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxV1);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxV1);
                                    }
                                }
                                break;
                            case "2":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxV2);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxV2);
                                    }
                                }
                                break;
                            case "3":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxV3);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxV3);
                                    }
                                }
                                break;
                            case "4":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxV4);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxV4);
                                    }
                                }
                                break;
                            case "5":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxV5);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxV5);
                                    }
                                }
                                break;
                            case "6":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxV6);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxV6);
                                    }
                                }
                                break;
                        }
                    }
                    else if (message.Contains("getPump:"))
                    {
                        string[] arguments = message.Split(delimiterChars);
                        switch (arguments[1])
                        {
                            case "1":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxP1);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxP1);
                                    }
                                }
                                break;
                            case "2":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxP2);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxP2);
                                    }
                                }
                                break;
                            case "3":
                                {
                                    if (arguments[2] == "On")
                                    {
                                        SetText("1", textBoxP3);
                                    }
                                    else
                                    {
                                        SetText("0", textBoxP3);
                                    }
                                }
                                break;                            
                        }
                    }
                    else
                    {
                        SetText(message, textBoxFLDMsg);
                    }

                }
                catch (TimeoutException) { }
            }
        }

        public void TargetMonitor()
        {
            while (true)
            {
                while (monitorPoll == true)
                {
                    serMut.WaitOne();

                    byte[] msg = new byte[1];

                    // Read Temperature
                    msg[0] = Command.GET_TEMP;
                    sPort.Write(msg, 0, 1);

                    Thread.Sleep(10);

                    // Get Vaccum Pressure
                    msg[0] = Command.GET_PRESS;
                    sPort.Write(msg, 0, 1);

                    Thread.Sleep(10);

                    if (sensirionPoll == true)
                    {
                        // Read Sensirion Sensor
                        msg[0] = Command.READ_SENSIRION;
                        sPort.Write(msg, 0, 1);

                        Thread.Sleep(100);
                    }

                    // Get Sample Pressure
                    msg[0] = Command.GET_SAMPLE;
                    sPort.Write(msg, 0, 1);

                    Thread.Sleep(10);

                    // Get Valves
                    var buffer = new byte[sizeof(float)];
                    var buffer2 = new float[1];
                    for (int n = 1; n < 7; n++)
                    {
                        msg[0] = Command.GET_VALVE;
                        sPort.Write(msg, 0, 1);

                        buffer2[0] = (float)n;
                        Buffer.BlockCopy(buffer2, 0, buffer, 0, buffer.Length);
                        sPort.Write(buffer, 0, buffer.Length);
                        Thread.Sleep(10);
                    }

                    // Get Pumps
                    for (int n = 1; n < 4; n++)
                    {
                        msg[0] = Command.GET_PUMP;
                        sPort.Write(msg, 0, 1);

                        buffer2[0] = (float)n;
                        Buffer.BlockCopy(buffer2, 0, buffer, 0, buffer.Length);
                        sPort.Write(buffer, 0, buffer.Length);
                        Thread.Sleep(10);
                    }

                    // Get Sensors
                    for (int n = 1; n < 21; n++)
                    {
                        msg[0] = Command.GET_SENSOR;
                        sPort.Write(msg, 0, 1);

                        buffer2[0] = (float)n;
                        Buffer.BlockCopy(buffer2, 0, buffer, 0, buffer.Length);
                        sPort.Write(buffer, 0, buffer.Length);
                        Thread.Sleep(10);
                    }

                    serMut.ReleaseMutex();

                    Thread.Sleep(10);

                }
            }
        }

        private void SetText(string text, TextBox textBox)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (textBox.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(SetText);
                this.Invoke(d, new object[] { text, textBox });
            }
            else
            {
                textBox.Text = text;
            }
        }

        private void ChangeButtonColor(Color color, Button button)
        {
            // InvokeRequired required compares the thread ID of the
            // calling thread to the thread ID of the creating thread.
            // If these threads are different, it returns true.
            if (button.InvokeRequired)
            {
                ChangeButtonColorCallback d = new ChangeButtonColorCallback(ChangeButtonColor);
                this.Invoke(d, new object[] { color, button });
            }
            else
            {
                button.ForeColor = color;
            }
        }

        private void comboBoxCOMPorts_MouseClick(object sender, MouseEventArgs e)
        {
            comboBoxCOMPorts.Items.Clear();
            comboBoxCOMPorts.Items.AddRange(SerialPort.GetPortNames());
        }

        private void comboBoxCOMPorts_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (sPort.IsOpen)
            {
                sPort.Close();
            }

            // Try to connect to selected COM Port
            sPort.PortName = comboBoxCOMPorts.Text;
            sPort.BaudRate = 512000;

            try
            {
                sPort.Open();
            }
            catch (Exception exc)
            {
                MessageBox.Show(exc.Message, "FLD Test App ERROR!!!");
            }

            if (sPort.IsOpen)
            {
                labelCOMPorts.Text = "CONNECTED";
                labelCOMPorts.Font = new Font(labelCOMPorts.Font, FontStyle.Bold);
                labelCOMPorts.ForeColor = Color.Green;

                // Start comms listener
                readThread = new Thread(ReadSerialPort);
                readThread.IsBackground = true;
                readThread.Start();

                // Start monitoring
                monitorThread = new Thread(TargetMonitor);
                monitorThread.IsBackground = true;
                monitorThread.Start();
            }
        }

        private void buttonDummy_Click(object sender, EventArgs e)
        {
            if (sPort.IsOpen != true) return;


            // Send Message to the FLD Board
            serMut.WaitOne();
            byte[] msg = new byte[1];
            msg[0] = Command.DUMMY_MESSAGE;
            sPort.Write(msg, 0, 1);
            serMut.ReleaseMutex();
        }

        private void groupBoxFLDCommands_Enter(object sender, EventArgs e)
        {

        }

        private void buttonSendCmd_Click(object sender, EventArgs e)
        {
            if (sPort.IsOpen != true) return;


            char[] delimiterChars = { ' '};

            string[] args = comboBoxCommand.Text.Split(delimiterChars);

            // Send Message to the FLD Board

            serMut.WaitOne();

            if ((args.Length == 1) && takes_args)
            {
                MessageBox.Show("Command takes arguments", "FLD Test App");
                return;
            }
            sPort.Write(globmsg, 0, 1);

            int n = 0;
            float[] arg = new float[10];
            foreach (string s in args)
            {
                if (n == 0)
                {
                    n++;
                    continue;
                }
                // create a byte array and copy the floats into it...
                arg[n - 1] = float.Parse(s);
                n++;
            }
            var buffer = new byte[(n - 1) * sizeof(float)];
            Buffer.BlockCopy(arg, 0, buffer, 0, buffer.Length);
            sPort.Write(buffer, 0, buffer.Length);

            serMut.ReleaseMutex();
        }
 
        private void comboBoxCommand_SelectedIndexChanged(object sender, EventArgs e)
        {
            globmsg[0] = ((Command)(comboBoxCommand.SelectedValue)).Code;
            takes_args = ((Command)(comboBoxCommand.SelectedValue)).Args;
        }

        private void buttonPoll_Click(object sender, EventArgs e)
        {
            if (sPort.IsOpen != true) return;


            if (buttonPoll.ForeColor == Color.Red)
            {
                buttonPoll.ForeColor = Color.Green;
                monitorPoll = true;
            }
            else
            {
                buttonPoll.ForeColor = Color.Red;
                monitorPoll = false;
                sensirionPoll = false;
            }
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void comboBoxCommand_KeyPress(object sender, KeyPressEventArgs e)
        {
                
        }

        private void comboBoxCommand_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                buttonSendCmd_Click(this, null);
                e.SuppressKeyPress = true;
            }
        }

        private void buttonExecMode_Click(object sender, EventArgs e)
        {
            if (sPort.IsOpen != true) return;
            if (listBoxModes.SelectedIndex == -1) return;

            // Send Message to the FLD Board
            serMut.WaitOne();

            byte[] msg = new byte[1];
            msg[0] = Command.EXEC_MODE;
            sPort.Write(msg, 0, 1);

            msg[0] = (byte)listBoxModes.SelectedIndex;
            sPort.Write(msg, 0, 1);

            buttonExecMode.ForeColor = Color.Red;

            serMut.ReleaseMutex();
        }

        private void buttonEstop_Click(object sender, EventArgs e)
        {
            if (sPort.IsOpen != true)
            {
                MessageBox.Show("Not connected to FLD board", "ERROR!!!");
                return;
            }


            // Send Message to the FLD Board
            serMut.WaitOne();
            byte[] msg = new byte[1];
            msg[0] = Command.E_STOP;
            sPort.Write(msg, 0, 1);
            serMut.ReleaseMutex();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            
        }

        private void buttonIncreaseFlowRate_Click(object sender, EventArgs e)
        {
            if (sPort.IsOpen != true) return;

            // Send Message to the FLD Board
            serMut.WaitOne();

            byte[] msg = new byte[1];
            msg[0] = Command.INCREASE_FLOW_RATE;
            sPort.Write(msg, 0, 1);

            // Percentage of travel
            var buffer2 = new float[1];
            var buffer = new byte[sizeof(float)];

            buffer2[0] = 1000;

            Buffer.BlockCopy(buffer2, 0, buffer, 0, buffer.Length);
            sPort.Write(buffer, 0, buffer.Length);

            serMut.ReleaseMutex();
        }

        private void buttonDecreaseFlowRate_Click(object sender, EventArgs e)
        {
            if (sPort.IsOpen != true) return;

            // Send Message to the FLD Board
            serMut.WaitOne();

            byte[] msg = new byte[1];
            msg[0] = Command.DECREASE_FLOW_RATE;
            sPort.Write(msg, 0, 1);

            // Percentage of travel
            var buffer2 = new float[1];
            var buffer = new byte[sizeof(float)];

            buffer2[0] = 1000;

            Buffer.BlockCopy(buffer2, 0, buffer, 0, buffer.Length);
            sPort.Write(buffer, 0, buffer.Length);
            
            serMut.ReleaseMutex();
        }

        private void buttonSetFlowRate_Click(object sender, EventArgs e)
        {
            if (sPort.IsOpen != true) return;

            // Send Message to the FLD Board
            serMut.WaitOne();

            byte[] msg = new byte[1];
            msg[0] = Command.SET_FLOW_RATE;
            sPort.Write(msg, 0, 1);

            // Percentage of travel (0 - 1000)
            var buffer2 = new float[1];
            var buffer = new byte[sizeof(float)];

            buffer2[0] = float.Parse(textBoxSetFlowRate.Text);

            Buffer.BlockCopy(buffer2, 0, buffer, 0, buffer.Length);
            sPort.Write(buffer, 0, buffer.Length);

            serMut.ReleaseMutex();
        }

        private void buttonSensEnable_Click(object sender, EventArgs e)
        {
            if (sensirionPoll == true)
            {
                sensirionPoll = false;
            }
            else
            {
                sensirionPoll = true;
            }
        }
    }

    public class Command
    {
        // MSG Codes
        public const byte LOAD_MODE_TABLE = 0x11;
        public const byte DUMMY_MESSAGE = 0x12;
        public const byte E_STOP = 0x13;
        public const byte SET_PRESS = 0x14;
        public const byte GET_PRESS = 0x15;
        public const byte SET_PRESS_TOL = 0x16;
        public const byte GET_PRESS_TOL = 0x17;
        public const byte SET_REF_TEMP = 0x18;
        public const byte GET_REF_TEMP = 0x34;
        public const byte GET_TEMP = 0x19;
        public const byte SET_TEMP_FACTOR = 0x1A;
        public const byte GET_TEMP_FACTOR = 0x1B;
        public const byte EXEC_MODE = 0x1C;
        public const byte SET_CAL_VALUES = 0x1D;
        public const byte GET_CAL_VALUES = 0x26;
        public const byte GET_SENSOR = 0x1E;
        public const byte GET_VALVE = 0x1F;
        public const byte GET_PUMP = 0x20;
        public const byte SET_VALVE = 0x21;
        public const byte SET_PUMP = 0x22;
        public const byte SET_LASER = 0x23;
        public const byte GET_LASER = 0x24;
        public const byte SET_LASER_DELAY = 0x25;
        public const byte INCREASE_FLOW_RATE = 0x29;
        public const byte DECREASE_FLOW_RATE = 0x27;
        public const byte GET_SAMPLE = 0x28;
        public const byte SET_FLOW_RATE = 0x30;
        public const byte READ_SENSIRION = 0x33;
        public const byte SET_CONTROLLER = 0x35;    //zrw
        public const byte SET_PID = 0x36;    //zrw
        public const byte GET_PID = 0x37;    //zrw
        public const byte GET_SIT_POS = 0x40;    //zrw
        public const byte GET_SIT_TOL = 0x42;    //zrw
        public const byte SET_SIT_TOL = 0x41;    //zrw
        public const byte CALIBRATE_SIT = 0x43;    //zrw
        public const byte MOVE_SIT = 0x44;    //zrw

        private string msg;
        private byte code;
        private bool args;

        public string Msg
        {
            get
            {
                return msg;
            }
            set
            {
                msg = value;
            }
        }

        public byte Code
        { 
            get
            {
                return code;
            }
            set
            {
                code = value;
            }
        }

        public bool Args
        {
            get
            {
                return args;
            }
        }

        public Command(String msg, byte code, bool args)
        {
            this.msg = msg;
            this.code = code;
            this.args = args;
        }

    }

    

}

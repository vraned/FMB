#pragma once
#using "..\..\..\Application\CyUSB.dll" 

namespace Bulkloop_VCPP {

	using namespace System;
	using namespace System::IO;
	using namespace System::Text;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace CyUSB;
	
	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		static const int VID = 0x04B4;
		static const int PID = 0x00F0;
		USBDeviceList^ usbDevices;																		// dynamic list of USBdevcies bound to CyUSB.sys
		CyUSBDevice^ dev;                                                                               
		CyUSBDevice^ myDevice;                                                                          // myDevice represents the bulkloop device in this example
		CyBulkEndPoint^ BulkInEndPt;		                                                            // We need one Bulk IN endpoint                
		CyBulkEndPoint^ BulkOutEndPt;		                                                            // 'We need one Bulk OUT endpoint             
		
		static Thread^ tXfers;																			// Thread where data transfers are handled
		static bool bRunning = false;																	// Variable determining whether tXfers thread is running or not
		static int value,value1,value2,value3;			
		static int byteCount = 0;
		static long outCount, inCount;																	// Number of bytes transferred in out and in direction 
		static long passCount, failCount;																	// Number of transfers PASSED and FAILED
		static int XFERSIZE = 1024;																// Transfers happen in chunks of 512 bytes in case of High speed; and 1024 bytes in case of superspeed

		static cli::array<Byte,1>^ outData = gcnew array<Byte,1>(XFERSIZE);								// OUT transfers send data in the outData buffer
		static cli::array<Byte,1>^ inData = gcnew array<Byte,1>(XFERSIZE);								// IN transfers send data into the inData buffer
	private: System::Windows::Forms::TextBox^  NumTransfersFailed;
	public:

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::TextBox^  NumTransfersPassed;
	public:

	public:


		// These 2 needed for TransfersThread to update the UI
		delegate void UpdateUICallback();																// Setup the callback routine for updating the UI
		UpdateUICallback^ updateUI;
       		
		Form1(void)
		{
			InitializeComponent();
			//Create a list of CYUSB devices
			usbDevices = gcnew USBDeviceList(CyConst::DEVICES_CYUSB);									// All devices served by CyUSB.sys are added to the list usbDevices
			
			// Declaration of event handlers - Deviceattached and DeviceRemoved 
			usbDevices->DeviceAttached += gcnew System::EventHandler(this,&Form1::usbDevices_DeviceAttached);		   // Eventhandler assigned to DeviceAttached to handle the event when a device is attached
			usbDevices->DeviceRemoved += gcnew System::EventHandler(this,&Form1::usbDevices_DeviceRemoved);            // Eventhandler assigned to DeviceRemoved to handle the event when a device is removed
			RefreshDevice();

			updateUI = gcnew UpdateUICallback(this,&Form1::StatusUpdate);                               // Setup the callback routine for updating the UI
			tXfers = gcnew Thread(gcnew ThreadStart(this,&Form1::TransfersThread));
			tXfers->IsBackground = true;
			tXfers->Priority = ThreadPriority::Highest;

			bRunning= false;
		}
		/* Summary
		 This is the event handler for Device Attachment event.
		*/
	       void usbDevices_DeviceAttached(Object^ sender, EventArgs^ e)													// Eventhandler assigned to DeviceAttached
	       {
		    RefreshDevice();		
	       }
	       /* Summary
		This is the event handler for Device removal event.
	       */
	       void usbDevices_DeviceRemoved(Object^ sender, EventArgs^ e)													// Eventhandler assigned to DeviceRemoved
	      {
		    RefreshDevice();													
	      }

	       /*Summary
		 The function gets the device with VID=04b4 and PID=00F0; updates the Status label and Treeview
	       */
		void RefreshDevice()
	       {
		    myDevice = (static_cast<CyUSBDevice^>(usbDevices[VID, PID]));										// Instantiating myDevice with handle to the device of interest (bulkloop device in this example)
		    if (!myDevice)																							// Bulkloop device if not found
		    {    StatusLabel->Text = "Bulkloop Device not connected";
			 StartBtn->Enabled = false;
			 StartBtn->Text = "Start";
			 StartBtn->BackColor = System::Drawing::Color::Aquamarine;
		    }
		    else																									// Bulkloop device if not found
		    {
			 StatusLabel->Text = "Bulkloop Device connected";
			 StartBtn->Enabled = true;
			 BulkOutEndPt = static_cast<CyBulkEndPoint^>( myDevice->EndPointOf(0x02));							// Instantiating the endpoints
			 BulkInEndPt = static_cast<CyBulkEndPoint^>(myDevice->EndPointOf(0x86));
		    }

		    // Clear and update TreeView
		    DeviceTreeView->Nodes->Clear();
		    for(int i=0 ; i < usbDevices->Count ; i++)
		    { 
			   dev = (static_cast<CyFX2Device^>(usbDevices[i]));														
			   DeviceTreeView->Nodes->Add(dev->Tree);																	
		     }
			   
	        }
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label6;
	protected: 
	private: System::Windows::Forms::TextBox^  ValBox;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  StatusLabel;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  EPLabel;
	private: System::Windows::Forms::Button^  StartBtn;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  NumBytesIn;
	private: System::Windows::Forms::TextBox^  NumBytesOut;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::TextBox^  InDataBox;
	private: System::Windows::Forms::TextBox^  OutdataBox;
	internal: System::Windows::Forms::TreeView^  DeviceTreeView;
	private: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->ValBox = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->StatusLabel = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->EPLabel = (gcnew System::Windows::Forms::Label());
			this->StartBtn = (gcnew System::Windows::Forms::Button());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->NumBytesIn = (gcnew System::Windows::Forms::TextBox());
			this->NumBytesOut = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->InDataBox = (gcnew System::Windows::Forms::TextBox());
			this->OutdataBox = (gcnew System::Windows::Forms::TextBox());
			this->DeviceTreeView = (gcnew System::Windows::Forms::TreeView());
			this->NumTransfersFailed = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->NumTransfersPassed = (gcnew System::Windows::Forms::TextBox());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(399, 80);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(85, 13);
			this->label6->TabIndex = 57;
			this->label6->Text = L"Data Byte(Hex) :";
			// 
			// ValBox
			// 
			this->ValBox->Location = System::Drawing::Point(493, 77);
			this->ValBox->MaxLength = 8;
			this->ValBox->Name = L"ValBox";
			this->ValBox->Size = System::Drawing::Size(85, 20);
			this->ValBox->TabIndex = 56;
			this->ValBox->Text = L"12345678";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->StatusLabel);
			this->groupBox1->Location = System::Drawing::Point(596, 10);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(287, 46);
			this->groupBox1->TabIndex = 54;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Status";
			// 
			// StatusLabel
			// 
			this->StatusLabel->AutoSize = true;
			this->StatusLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->StatusLabel->Location = System::Drawing::Point(53, 16);
			this->StatusLabel->Name = L"StatusLabel";
			this->StatusLabel->Size = System::Drawing::Size(87, 13);
			this->StatusLabel->TabIndex = 1;
			this->StatusLabel->Text = L"Device Status";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->EPLabel);
			this->groupBox2->Location = System::Drawing::Point(286, 10);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(276, 45);
			this->groupBox2->TabIndex = 55;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"EndPoint Pair OUT/IN";
			// 
			// EPLabel
			// 
			this->EPLabel->AutoSize = true;
			this->EPLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->EPLabel->Location = System::Drawing::Point(85, 17);
			this->EPLabel->Name = L"EPLabel";
			this->EPLabel->Size = System::Drawing::Size(117, 15);
			this->EPLabel->TabIndex = 0;
			this->EPLabel->Text = L"EP2 OUT/ EP6 IN";
			// 
			// StartBtn
			// 
			this->StartBtn->Enabled = false;
			this->StartBtn->Location = System::Drawing::Point(775, 69);
			this->StartBtn->Name = L"StartBtn";
			this->StartBtn->Size = System::Drawing::Size(120, 29);
			this->StartBtn->TabIndex = 53;
			this->StartBtn->Text = L"START";
			this->StartBtn->UseVisualStyleBackColor = true;
			this->StartBtn->Click += gcnew System::EventHandler(this, &Form1::StartBtn_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(596, 382);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(156, 13);
			this->label3->TabIndex = 52;
			this->label3->Text = L"Number of Bytes Transferred IN";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(281, 385);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(168, 13);
			this->label2->TabIndex = 51;
			this->label2->Text = L"Number of Bytes Transferred OUT";
			// 
			// NumBytesIn
			// 
			this->NumBytesIn->BackColor = System::Drawing::SystemColors::Info;
			this->NumBytesIn->Location = System::Drawing::Point(775, 378);
			this->NumBytesIn->Name = L"NumBytesIn";
			this->NumBytesIn->ReadOnly = true;
			this->NumBytesIn->Size = System::Drawing::Size(119, 20);
			this->NumBytesIn->TabIndex = 50;
			// 
			// NumBytesOut
			// 
			this->NumBytesOut->BackColor = System::Drawing::SystemColors::Info;
			this->NumBytesOut->Location = System::Drawing::Point(467, 379);
			this->NumBytesOut->Name = L"NumBytesOut";
			this->NumBytesOut->ReadOnly = true;
			this->NumBytesOut->Size = System::Drawing::Size(119, 20);
			this->NumBytesOut->TabIndex = 49;
			this->NumBytesOut->TextChanged += gcnew System::EventHandler(this, &Form1::NumBytesOut_TextChanged);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(593, 85);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(104, 13);
			this->label5->TabIndex = 48;
			this->label5->Text = L"Bytes Transferred IN";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(273, 91);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(116, 13);
			this->label4->TabIndex = 47;
			this->label4->Text = L"Bytes Transferred OUT";
			// 
			// InDataBox
			// 
			this->InDataBox->BackColor = System::Drawing::SystemColors::Info;
			this->InDataBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->InDataBox->Location = System::Drawing::Point(596, 107);
			this->InDataBox->Multiline = true;
			this->InDataBox->Name = L"InDataBox";
			this->InDataBox->ReadOnly = true;
			this->InDataBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->InDataBox->Size = System::Drawing::Size(328, 260);
			this->InDataBox->TabIndex = 46;
			// 
			// OutdataBox
			// 
			this->OutdataBox->BackColor = System::Drawing::SystemColors::Info;
			this->OutdataBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->OutdataBox->Location = System::Drawing::Point(264, 107);
			this->OutdataBox->Multiline = true;
			this->OutdataBox->Name = L"OutdataBox";
			this->OutdataBox->ReadOnly = true;
			this->OutdataBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->OutdataBox->Size = System::Drawing::Size(326, 260);
			this->OutdataBox->TabIndex = 45;
			// 
			// DeviceTreeView
			// 
			this->DeviceTreeView->Location = System::Drawing::Point(11, 10);
			this->DeviceTreeView->Name = L"DeviceTreeView";
			this->DeviceTreeView->Size = System::Drawing::Size(244, 401);
			this->DeviceTreeView->TabIndex = 44;
			// 
			// NumTransfersFailed
			// 
			this->NumTransfersFailed->BackColor = System::Drawing::SystemColors::Info;
			this->NumTransfersFailed->Location = System::Drawing::Point(775, 402);
			this->NumTransfersFailed->Name = L"NumTransfersFailed";
			this->NumTransfersFailed->ReadOnly = true;
			this->NumTransfersFailed->Size = System::Drawing::Size(119, 20);
			this->NumTransfersFailed->TabIndex = 58;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(595, 405);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(91, 13);
			this->label1->TabIndex = 59;
			this->label1->Text = L"Transfers FAILED";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(281, 405);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(97, 13);
			this->label7->TabIndex = 60;
			this->label7->Text = L"Transfers PASSED";
			// 
			// NumTransfersPassed
			// 
			this->NumTransfersPassed->BackColor = System::Drawing::SystemColors::Info;
			this->NumTransfersPassed->Location = System::Drawing::Point(467, 405);
			this->NumTransfersPassed->Name = L"NumTransfersPassed";
			this->NumTransfersPassed->ReadOnly = true;
			this->NumTransfersPassed->Size = System::Drawing::Size(119, 20);
			this->NumTransfersPassed->TabIndex = 61;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(936, 432);
			this->Controls->Add(this->NumTransfersPassed);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->NumTransfersFailed);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->ValBox);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->StartBtn);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->NumBytesIn);
			this->Controls->Add(this->NumBytesOut);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->InDataBox);
			this->Controls->Add(this->OutdataBox);
			this->Controls->Add(this->DeviceTreeView);
			this->Name = L"Form1";
			this->Text = L"Bulkloop_VCPP";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		/*Summary
		  Event handler for Click event of Start/ Stop button. Starts or stops the thread and accordingly updates the text of the StartBtn.
		*/
		private: System::Void StartBtn_Click(System::Object^  sender, System::EventArgs^  e) {
	    
		    if ((!BulkOutEndPt) && (!BulkInEndPt))							// check for NULL endpoints
		    {
			    MessageBox::Show( "No EP2 BULK OUT and EP6 BULK IN endpoints", "Bulkloop_VCPP", MessageBoxButtons::OK);
			    StartBtn->Text = "Start";
			    StartBtn->BackColor = System::Drawing::Color::Aquamarine;
			    goto my_label;
		    }
		    else
		    {
		    	    if (!BulkOutEndPt) 
			     {
				     MessageBox::Show( "No EP2 BULK OUT endpoint", "Bulkloop_VCPP", MessageBoxButtons::OK);
				     StartBtn->Text = "Start";
				     StartBtn->BackColor = System::Drawing::Color::Aquamarine;
				     goto my_label;
			     }
			     if (!BulkInEndPt)
			     {
				    MessageBox::Show( "No EP6 BULK IN endpoints", "Bulkloop_VCPP", MessageBoxButtons::OK);
				    StartBtn->Text = "Start";
			            StartBtn->BackColor = System::Drawing::Color::Aquamarine;
				    goto my_label;
			     }
		    }

	            if(bRunning == false)
		    {
			     if(ValBox->Text != "")
			     {						 
				    Encoding^ encoding = Encoding::UTF7;
				    array<Byte>^ byteArray = encoding->GetBytes(ValBox->Text);
				    byteCount = encoding->GetByteCount(ValBox->Text);
				    value = 0;
				    value1 = 0;
				    value2 = 0;
				    value3 = 0;
				    if (byteCount >= 1)
				    {
					  value = Convert::ToInt32(encoding->GetString(byteArray, 0, 1), 16);
				    }
	                             if (byteCount >= 2) 
		           	     {
					   value = Convert::ToInt32(encoding->GetString(byteArray, 0, 2), 16);
				      }
	                             if (byteCount >= 3)
				     {
			                  value1 = Convert::ToInt32(encoding->GetString(byteArray, 2, 1), 16);
				     }       
				     if (byteCount >= 4) 
				     {
					  value1 = Convert::ToInt32(encoding->GetString(byteArray, 2, 2), 16);
				     }
	                             if (byteCount >= 5)
				     {
					  value2 = Convert::ToInt32(encoding->GetString(byteArray, 4, 1), 16);
				     }
	    	                     if (byteCount >= 6)
				     {
					  value2 = Convert::ToInt32(encoding->GetString(byteArray, 4, 2), 16);
				     }
		                     if (byteCount >= 7) 
				     {
	       	                           value3 = Convert::ToInt32(encoding->GetString(byteArray, 6, 1), 16);
				     }
			             if (byteCount >= 8) 
				     {
					   value3 = Convert::ToInt32(encoding->GetString(byteArray, 6, 2), 16);
				     }

			      }

			      outCount = 0;
			      inCount = 0;
				  failCount = 0;
				  passCount = 0;

			      OutdataBox->Text = "";
			      InDataBox->Text = "";

			      bRunning = true;
			      StartBtn->Text = "Stop";
			      StartBtn->BackColor = System::Drawing::Color::Pink;

			      //creates new thread
			      tXfers = gcnew Thread(gcnew ThreadStart(this,&Form1::TransfersThread));
			      tXfers->IsBackground = true;
			      tXfers->Priority = ThreadPriority::Highest;
			      //Starts the new thread
			      tXfers->Start();
		    }
		    else
		    {
			      //Makes the thread stop and aborts the thread
			      bRunning = false;
		              StartBtn->Text = "Start";
			      StartBtn->BackColor = System::Drawing::Color::Aquamarine;
			      if (!tXfers) 
			      {
				    return;
			      }
			      if (tXfers->IsAlive) 
			      {
				   tXfers->Abort();
				   tXfers->Join();
				   tXfers = nullptr;
				   BulkOutEndPt->Reset();
                	      }
		     }
		     my_label:
			StartBtn->Text = "Start";
			StartBtn->BackColor = System::Drawing::Color::Aquamarine;

		 }

		 /* Summary
		This thread is initiated on start button click. Runs the thread; invokes the data transfers and invokes the StatusUpdate to update the UI 
		 */

		public: void TransfersThread(void)
	       {
		        int xferLen = XFERSIZE;

			bool bResult = true;
			if (myDevice->bSuperSpeed)
				XFERSIZE = 1024;
			else if (myDevice->bHighSpeed)
				XFERSIZE = 512;            
			for (; bRunning && bResult; )
			{
			       SetOutputData();                                                         //this function sets the bytes in outData to be transferred to OUT EP
			       xferLen = XFERSIZE;
			       //calls the XferData function for bulk transfer(OUT) in the cyusb.dll
			       bResult = BulkOutEndPt->XferData(outData, xferLen);
			       outCount += xferLen;

			      if (bResult)
			      {
				    //calls the XferData function for bulk transfer(IN) in the cyusb.dll
				    bResult = BulkInEndPt->XferData(inData, xferLen);
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
			    this->Invoke(updateUI);                
			 }

			// Call StatusUpdate() in the main thread
			this->Invoke(updateUI);
			my_label1:
			    bRunning = false;
			    
		}

		/* Summary
		  Takes the user-given data in the ‘Data Bytes(Hex)’ field of the application user interface and fills the ‘outData[]’ buffer with these bytes 
		*/
		void SetOutputData(void)
		{
		        for (int i = 0; i < XFERSIZE; i++)
			{
			    if((byteCount >= 1) && (i < XFERSIZE)) outData[i] = (Byte)value;
			    if((byteCount >= 3) && (++i < XFERSIZE)) outData[i] = (Byte)value1;
			    if((byteCount >= 5) && (++i < XFERSIZE)) outData[i] = (Byte)value2;
			    if((byteCount >= 7) && (++i < XFERSIZE)) outData[i] = (Byte)value3;
			}
                         
		}

		/* Summary
		This is the call back function for updating the UI(user interface) and is called from TransfersThread. This updates the OutdataBox,InDataBox
		with the Bytes transferred; and NumBytesOut,NumBytesIn with the number of Bytes transferred.
         	*/
		 void StatusUpdate()
		{
			StringBuilder^ dataStrOut = gcnew StringBuilder();
			StringBuilder^ dataStrIn = gcnew StringBuilder();
            
			if (bRunning)
			{
			    for (int i = 0; i < XFERSIZE; i++)
			    {
				    if ((i % 16) == 0) dataStrOut->Append(String::Format("\r\n{0:X4}", i));
				    dataStrOut->Append(String::Format(" {0:X2}", outData[i]));
			    }
			
			    dataStrOut->Append(String::Format("\r\n"));
			    OutdataBox->Text += Convert::ToString(dataStrOut);                                   //This updates the OutdataBox with bytes transferred OUT

			    for (int i = 0; i < XFERSIZE; i++)
			    {
				    if ((i % 16) == 0) dataStrIn->Append(String::Format("\r\n{0:X4}", i));
				    dataStrIn->Append(String::Format(" {0:X2}", inData[i]));
			    }
			    dataStrIn->Append(String::Format("\r\n"));
			    InDataBox->Text += Convert::ToString(dataStrIn);                                      //This updates the InDataBox with bytes transferred IN
			}


			NumBytesOut->Text = outCount.ToString();                                          //This updates the NumBytesOut with number of bytes transferred OUT
			NumBytesIn->Text = inCount.ToString();                                           //This updates the NumBytesIn with number of bytes transferred IN
			NumTransfersFailed->Text = failCount.ToString();                                          //This updates the NumTransfersFailed with number of transfers FAILED
			NumTransfersPassed->Text = passCount.ToString();                                          //This updates the NumTransfersPassed with number of bytes transferred PASSED
			Refresh();

			StartBtn->Text = bRunning ? "Stop" : "Start";
			StartBtn->BackColor = bRunning ? System::Drawing::Color::Pink : System::Drawing::Color::Aquamarine;
			        
		}
		/*Summary
		  Eventhandler for Form1 Close event
		*/
		private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {

			 // If close was selected while running the loopback, shut it down.
			if (bRunning)
			    StartBtn_Click(this, nullptr);
		}
		
private: System::Void NumBytesOut_TextChanged(System::Object^  sender, System::EventArgs^  e) {
}
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
}
};
}


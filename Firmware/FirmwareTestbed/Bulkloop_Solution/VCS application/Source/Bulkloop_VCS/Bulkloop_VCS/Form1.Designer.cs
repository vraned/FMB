using CyUSB;

namespace Bulkloop_VCS
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label6 = new System.Windows.Forms.Label();
            this.ValBox = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.StatusLabel = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.EPLabel = new System.Windows.Forms.Label();
            this.StartBtn = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.NumBytesIn = new System.Windows.Forms.TextBox();
            this.NumBytesOut = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.InDataBox = new System.Windows.Forms.TextBox();
            this.OutdataBox = new System.Windows.Forms.TextBox();
            this.DeviceTreeView = new System.Windows.Forms.TreeView();
            this.label7 = new System.Windows.Forms.Label();
            this.NumTransfersPassed = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.NumTransfersFailed = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(396, 81);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(85, 13);
            this.label6.TabIndex = 71;
            this.label6.Text = "Data Byte(Hex) :";
            // 
            // ValBox
            // 
            this.ValBox.Location = new System.Drawing.Point(490, 78);
            this.ValBox.MaxLength = 8;
            this.ValBox.Name = "ValBox";
            this.ValBox.Size = new System.Drawing.Size(85, 20);
            this.ValBox.TabIndex = 70;
            this.ValBox.Text = "12345678";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.StatusLabel);
            this.groupBox1.Location = new System.Drawing.Point(593, 11);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(287, 46);
            this.groupBox1.TabIndex = 68;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Status";
            // 
            // StatusLabel
            // 
            this.StatusLabel.AutoSize = true;
            this.StatusLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StatusLabel.Location = new System.Drawing.Point(53, 16);
            this.StatusLabel.Name = "StatusLabel";
            this.StatusLabel.Size = new System.Drawing.Size(87, 13);
            this.StatusLabel.TabIndex = 1;
            this.StatusLabel.Text = "Device Status";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.EPLabel);
            this.groupBox2.Location = new System.Drawing.Point(283, 11);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(276, 45);
            this.groupBox2.TabIndex = 69;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "EndPoint Pair OUT/IN";
            // 
            // EPLabel
            // 
            this.EPLabel.AutoSize = true;
            this.EPLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.EPLabel.Location = new System.Drawing.Point(85, 17);
            this.EPLabel.Name = "EPLabel";
            this.EPLabel.Size = new System.Drawing.Size(117, 15);
            this.EPLabel.TabIndex = 0;
            this.EPLabel.Text = "EP2 OUT/ EP6 IN";
            // 
            // StartBtn
            // 
            this.StartBtn.Enabled = false;
            this.StartBtn.Location = new System.Drawing.Point(772, 70);
            this.StartBtn.Name = "StartBtn";
            this.StartBtn.Size = new System.Drawing.Size(120, 29);
            this.StartBtn.TabIndex = 67;
            this.StartBtn.Text = "START";
            this.StartBtn.UseVisualStyleBackColor = true;
            this.StartBtn.Click += new System.EventHandler(this.StartBtn_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(593, 383);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(156, 13);
            this.label3.TabIndex = 66;
            this.label3.Text = "Number of Bytes Transferred IN";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(278, 386);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(168, 13);
            this.label2.TabIndex = 65;
            this.label2.Text = "Number of Bytes Transferred OUT";
            // 
            // NumBytesIn
            // 
            this.NumBytesIn.BackColor = System.Drawing.SystemColors.Info;
            this.NumBytesIn.Location = new System.Drawing.Point(772, 379);
            this.NumBytesIn.Name = "NumBytesIn";
            this.NumBytesIn.ReadOnly = true;
            this.NumBytesIn.Size = new System.Drawing.Size(119, 20);
            this.NumBytesIn.TabIndex = 64;
            // 
            // NumBytesOut
            // 
            this.NumBytesOut.BackColor = System.Drawing.SystemColors.Info;
            this.NumBytesOut.Location = new System.Drawing.Point(464, 380);
            this.NumBytesOut.Name = "NumBytesOut";
            this.NumBytesOut.ReadOnly = true;
            this.NumBytesOut.Size = new System.Drawing.Size(123, 20);
            this.NumBytesOut.TabIndex = 63;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(590, 86);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(104, 13);
            this.label5.TabIndex = 62;
            this.label5.Text = "Bytes Transferred IN";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(270, 92);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(116, 13);
            this.label4.TabIndex = 61;
            this.label4.Text = "Bytes Transferred OUT";
            // 
            // InDataBox
            // 
            this.InDataBox.BackColor = System.Drawing.SystemColors.Info;
            this.InDataBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.InDataBox.Location = new System.Drawing.Point(593, 108);
            this.InDataBox.Multiline = true;
            this.InDataBox.Name = "InDataBox";
            this.InDataBox.ReadOnly = true;
            this.InDataBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.InDataBox.Size = new System.Drawing.Size(328, 260);
            this.InDataBox.TabIndex = 60;
            // 
            // OutdataBox
            // 
            this.OutdataBox.BackColor = System.Drawing.SystemColors.Info;
            this.OutdataBox.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.OutdataBox.Location = new System.Drawing.Point(261, 108);
            this.OutdataBox.Multiline = true;
            this.OutdataBox.Name = "OutdataBox";
            this.OutdataBox.ReadOnly = true;
            this.OutdataBox.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.OutdataBox.Size = new System.Drawing.Size(326, 260);
            this.OutdataBox.TabIndex = 59;
            // 
            // DeviceTreeView
            // 
            this.DeviceTreeView.Location = new System.Drawing.Point(8, 11);
            this.DeviceTreeView.Name = "DeviceTreeView";
            this.DeviceTreeView.Size = new System.Drawing.Size(244, 401);
            this.DeviceTreeView.TabIndex = 58;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(276, 406);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(97, 13);
            this.label7.TabIndex = 72;
            this.label7.Text = "Transfers PASSED";
            // 
            // NumTransfersPassed
            // 
            this.NumTransfersPassed.BackColor = System.Drawing.SystemColors.Info;
            this.NumTransfersPassed.Location = new System.Drawing.Point(464, 406);
            this.NumTransfersPassed.Name = "NumTransfersPassed";
            this.NumTransfersPassed.ReadOnly = true;
            this.NumTransfersPassed.Size = new System.Drawing.Size(123, 20);
            this.NumTransfersPassed.TabIndex = 75;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(592, 406);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(91, 13);
            this.label1.TabIndex = 74;
            this.label1.Text = "Transfers FAILED";
            // 
            // NumTransfersFailed
            // 
            this.NumTransfersFailed.BackColor = System.Drawing.SystemColors.Info;
            this.NumTransfersFailed.Location = new System.Drawing.Point(772, 403);
            this.NumTransfersFailed.Name = "NumTransfersFailed";
            this.NumTransfersFailed.ReadOnly = true;
            this.NumTransfersFailed.Size = new System.Drawing.Size(119, 20);
            this.NumTransfersFailed.TabIndex = 73;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(931, 435);
            this.Controls.Add(this.NumTransfersPassed);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.NumTransfersFailed);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.ValBox);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.StartBtn);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.NumBytesIn);
            this.Controls.Add(this.NumBytesOut);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.InDataBox);
            this.Controls.Add(this.OutdataBox);
            this.Controls.Add(this.DeviceTreeView);
            this.Name = "Form1";
            this.Text = "Bulkloop_VCS";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox ValBox;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label StatusLabel;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label EPLabel;
        private System.Windows.Forms.Button StartBtn;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox NumBytesIn;
        private System.Windows.Forms.TextBox NumBytesOut;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox InDataBox;
        private System.Windows.Forms.TextBox OutdataBox;
        internal System.Windows.Forms.TreeView DeviceTreeView;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox NumTransfersPassed;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox NumTransfersFailed;
    }
}


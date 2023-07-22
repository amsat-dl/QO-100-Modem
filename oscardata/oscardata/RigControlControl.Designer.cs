namespace oscardata
{
    partial class RigControlControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.combo_radio = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tb_hamlibDetected = new System.Windows.Forms.TextBox();
            this.btnTestPtt = new System.Windows.Forms.Button();
            this.btnReleasePtt = new System.Windows.Forms.Button();
            this.tbRigDevice = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.btnGetFrequency = new System.Windows.Forms.Button();
            this.frequencyLabel = new System.Windows.Forms.Label();
            this.btnSave = new System.Windows.Forms.Button();
            this.hamlibLinkLabel = new System.Windows.Forms.LinkLabel();
            this.SuspendLayout();
            // 
            // combo_radio
            // 
            this.combo_radio.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.combo_radio.FormattingEnabled = true;
            this.combo_radio.Location = new System.Drawing.Point(111, 52);
            this.combo_radio.Name = "combo_radio";
            this.combo_radio.Size = new System.Drawing.Size(235, 21);
            this.combo_radio.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(22, 55);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(35, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Radio";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(22, 29);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(39, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Hamlib";
            // 
            // tb_hamlibDetected
            // 
            this.tb_hamlibDetected.Location = new System.Drawing.Point(111, 26);
            this.tb_hamlibDetected.Name = "tb_hamlibDetected";
            this.tb_hamlibDetected.ReadOnly = true;
            this.tb_hamlibDetected.Size = new System.Drawing.Size(235, 20);
            this.tb_hamlibDetected.TabIndex = 3;
            // 
            // btnTestPtt
            // 
            this.btnTestPtt.Location = new System.Drawing.Point(111, 127);
            this.btnTestPtt.Name = "btnTestPtt";
            this.btnTestPtt.Size = new System.Drawing.Size(75, 23);
            this.btnTestPtt.TabIndex = 4;
            this.btnTestPtt.Text = "Test PTT";
            this.btnTestPtt.UseVisualStyleBackColor = true;
            this.btnTestPtt.Click += new System.EventHandler(this.btnTestPtt_Click);
            // 
            // btnReleasePtt
            // 
            this.btnReleasePtt.Location = new System.Drawing.Point(192, 127);
            this.btnReleasePtt.Name = "btnReleasePtt";
            this.btnReleasePtt.Size = new System.Drawing.Size(93, 23);
            this.btnReleasePtt.TabIndex = 5;
            this.btnReleasePtt.Text = "Release PTT";
            this.btnReleasePtt.UseVisualStyleBackColor = true;
            this.btnReleasePtt.Click += new System.EventHandler(this.btnReleasePtt_Click);
            // 
            // tbRigDevice
            // 
            this.tbRigDevice.Location = new System.Drawing.Point(111, 79);
            this.tbRigDevice.Name = "tbRigDevice";
            this.tbRigDevice.Size = new System.Drawing.Size(235, 20);
            this.tbRigDevice.TabIndex = 7;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(22, 82);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(41, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Device";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(365, 82);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(142, 13);
            this.label4.TabIndex = 8;
            this.label4.Text = "e.g. COM4, 127.0.0.1:60002";
            this.label4.DoubleClick += new System.EventHandler(this.label4_DoubleClick);
            // 
            // btnGetFrequency
            // 
            this.btnGetFrequency.Location = new System.Drawing.Point(291, 127);
            this.btnGetFrequency.Name = "btnGetFrequency";
            this.btnGetFrequency.Size = new System.Drawing.Size(93, 23);
            this.btnGetFrequency.TabIndex = 9;
            this.btnGetFrequency.Text = "Get frequency";
            this.btnGetFrequency.UseVisualStyleBackColor = true;
            this.btnGetFrequency.Click += new System.EventHandler(this.btnGetFrequency_Click);
            // 
            // frequencyLabel
            // 
            this.frequencyLabel.AutoSize = true;
            this.frequencyLabel.Location = new System.Drawing.Point(390, 132);
            this.frequencyLabel.Name = "frequencyLabel";
            this.frequencyLabel.Size = new System.Drawing.Size(54, 13);
            this.frequencyLabel.TabIndex = 10;
            this.frequencyLabel.Text = "frequency";
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.btnSave.Location = new System.Drawing.Point(111, 352);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(75, 23);
            this.btnSave.TabIndex = 11;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // hamlibLinkLabel
            // 
            this.hamlibLinkLabel.AutoSize = true;
            this.hamlibLinkLabel.Location = new System.Drawing.Point(365, 29);
            this.hamlibLinkLabel.Name = "hamlibLinkLabel";
            this.hamlibLinkLabel.Size = new System.Drawing.Size(121, 13);
            this.hamlibLinkLabel.TabIndex = 12;
            this.hamlibLinkLabel.TabStop = true;
            this.hamlibLinkLabel.Text = "https://hamlib.github.io/";
            this.hamlibLinkLabel.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.hamlibLinkLabel_LinkClicked);
            // 
            // RigControlControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.hamlibLinkLabel);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.frequencyLabel);
            this.Controls.Add(this.btnGetFrequency);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.tbRigDevice);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.btnReleasePtt);
            this.Controls.Add(this.btnTestPtt);
            this.Controls.Add(this.tb_hamlibDetected);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.combo_radio);
            this.Name = "RigControlControl";
            this.Size = new System.Drawing.Size(1107, 427);
            this.Load += new System.EventHandler(this.RigControlControl_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox combo_radio;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tb_hamlibDetected;
        private System.Windows.Forms.Button btnTestPtt;
        private System.Windows.Forms.Button btnReleasePtt;
        private System.Windows.Forms.TextBox tbRigDevice;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button btnGetFrequency;
        private System.Windows.Forms.Label frequencyLabel;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.LinkLabel hamlibLinkLabel;
    }
}

namespace oscardata
{
    partial class Form1
    {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.timer_udpTX = new System.Windows.Forms.Timer(this.components);
            this.timer_udprx = new System.Windows.Forms.Timer(this.components);
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.ts_ip = new System.Windows.Forms.ToolStripStatusLabel();
            this.RXstatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.panel_constel = new System.Windows.Forms.Panel();
            this.timer_qpsk = new System.Windows.Forms.Timer(this.components);
            this.panel_txspectrum = new System.Windows.Forms.Panel();
            this.tabPage_ber = new System.Windows.Forms.TabPage();
            this.button_stopBERtest = new System.Windows.Forms.Button();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.button_startBERtest = new System.Windows.Forms.Button();
            this.rtb = new System.Windows.Forms.RichTextBox();
            this.tabPage_image = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.Panel();
            this.label_nextimage = new System.Windows.Forms.Label();
            this.cb_loop = new System.Windows.Forms.CheckBox();
            this.bt_rximages = new System.Windows.Forms.Button();
            this.button_loadimage = new System.Windows.Forms.Button();
            this.comboBox_quality = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.checkBox_big = new System.Windows.Forms.CheckBox();
            this.button_cancelimg = new System.Windows.Forms.Button();
            this.button_sendimage = new System.Windows.Forms.Button();
            this.label_rximage = new System.Windows.Forms.Label();
            this.label_tximage = new System.Windows.Forms.Label();
            this.pictureBox_rximage = new System.Windows.Forms.PictureBox();
            this.pictureBox_tximage = new System.Windows.Forms.PictureBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage_file = new System.Windows.Forms.TabPage();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.button2 = new System.Windows.Forms.Button();
            this.bt_openrxfile = new System.Windows.Forms.Button();
            this.label_rxfile = new System.Windows.Forms.Label();
            this.label_txfile = new System.Windows.Forms.Label();
            this.rtb_RXfile = new System.Windows.Forms.RichTextBox();
            this.rtb_TXfile = new System.Windows.Forms.RichTextBox();
            this.bt_file_send = new System.Windows.Forms.Button();
            this.bt_sendBinaryFile = new System.Windows.Forms.Button();
            this.bt_file_html = new System.Windows.Forms.Button();
            this.bt_file_ascii = new System.Windows.Forms.Button();
            this.tabPage_audio = new System.Windows.Forms.TabPage();
            this.groupBox7 = new System.Windows.Forms.GroupBox();
            this.rb_codec2 = new System.Windows.Forms.RadioButton();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.rb_opus = new System.Windows.Forms.RadioButton();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.pb_voice = new System.Windows.Forms.PictureBox();
            this.cb_digitalVoiceRXonly = new System.Windows.Forms.CheckBox();
            this.cb_digitalVoice = new System.Windows.Forms.CheckBox();
            this.cb_codecloop = new System.Windows.Forms.CheckBox();
            this.cb_switchtoLS = new System.Windows.Forms.CheckBox();
            this.cb_voiceloop = new System.Windows.Forms.CheckBox();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.pb_voiceCAPstatus = new System.Windows.Forms.PictureBox();
            this.pb_voicePBstatus = new System.Windows.Forms.PictureBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label9 = new System.Windows.Forms.Label();
            this.tb_mic = new System.Windows.Forms.TrackBar();
            this.tb_loadspeaker = new System.Windows.Forms.TrackBar();
            this.cb_loudspeaker = new System.Windows.Forms.ComboBox();
            this.label10 = new System.Windows.Forms.Label();
            this.cb_mic = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.tabPage_setup = new System.Windows.Forms.TabPage();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.cb_language = new System.Windows.Forms.ComboBox();
            this.cb_autostart = new System.Windows.Forms.CheckBox();
            this.bt_shutdown = new System.Windows.Forms.Button();
            this.tb_shutdown = new System.Windows.Forms.TextBox();
            this.bt_resetmodem = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.pb_audioCAPstatus = new System.Windows.Forms.PictureBox();
            this.pb_audioPBstatus = new System.Windows.Forms.PictureBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.tb_CAPvol = new System.Windows.Forms.TrackBar();
            this.tb_PBvol = new System.Windows.Forms.TrackBar();
            this.cb_audioPB = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.cb_audioCAP = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.cb_stampinfo = new System.Windows.Forms.CheckBox();
            this.tb_info = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.cb_announcement = new System.Windows.Forms.ComboBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.tb_callsign = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.cb_stampcall = new System.Windows.Forms.CheckBox();
            this.tabPage_about = new System.Windows.Forms.TabPage();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.cb_speed = new System.Windows.Forms.ComboBox();
            this.label_speed = new System.Windows.Forms.Label();
            this.timer_searchmodem = new System.Windows.Forms.Timer(this.components);
            this.progressBar_fifo = new System.Windows.Forms.ProgressBar();
            this.label_fifo = new System.Windows.Forms.Label();
            this.bt_blockinfo = new System.Windows.Forms.Button();
            this.progressBar_capfifo = new System.Windows.Forms.ProgressBar();
            this.label_capfifo = new System.Windows.Forms.Label();
            this.lb_rxsignal = new System.Windows.Forms.Label();
            this.lb_rxsync = new System.Windows.Forms.Label();
            this.pb_rxsync = new System.Windows.Forms.PictureBox();
            this.pb_rxsignal = new System.Windows.Forms.PictureBox();
            this.statusStrip1.SuspendLayout();
            this.tabPage_ber.SuspendLayout();
            this.tabPage_image.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_rximage)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_tximage)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPage_file.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabPage_audio.SuspendLayout();
            this.groupBox7.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            this.groupBox6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pb_voice)).BeginInit();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pb_voiceCAPstatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_voicePBstatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_mic)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_loadspeaker)).BeginInit();
            this.tabPage_setup.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pb_audioCAPstatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_audioPBstatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_CAPvol)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_PBvol)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.tabPage_about.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pb_rxsync)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_rxsignal)).BeginInit();
            this.SuspendLayout();
            // 
            // timer_udpTX
            // 
            this.timer_udpTX.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // timer_udprx
            // 
            this.timer_udprx.Tick += new System.EventHandler(this.timer_udprx_Tick);
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel,
            this.ts_ip,
            this.RXstatus});
            this.statusStrip1.Location = new System.Drawing.Point(0, 669);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1293, 22);
            this.statusStrip1.TabIndex = 4;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel
            // 
            this.toolStripStatusLabel.Name = "toolStripStatusLabel";
            this.toolStripStatusLabel.Size = new System.Drawing.Size(39, 17);
            this.toolStripStatusLabel.Text = "Status";
            // 
            // ts_ip
            // 
            this.ts_ip.ForeColor = System.Drawing.Color.Red;
            this.ts_ip.Name = "ts_ip";
            this.ts_ip.Size = new System.Drawing.Size(12, 17);
            this.ts_ip.Text = "?";
            // 
            // RXstatus
            // 
            this.RXstatus.Name = "RXstatus";
            this.RXstatus.Size = new System.Drawing.Size(58, 17);
            this.RXstatus.Text = "RX-Status";
            // 
            // panel_constel
            // 
            this.panel_constel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(220)))));
            this.panel_constel.Location = new System.Drawing.Point(11, 590);
            this.panel_constel.Name = "panel_constel";
            this.panel_constel.Size = new System.Drawing.Size(75, 75);
            this.panel_constel.TabIndex = 5;
            this.panel_constel.Paint += new System.Windows.Forms.PaintEventHandler(this.panel_constel_Paint);
            // 
            // timer_qpsk
            // 
            this.timer_qpsk.Enabled = true;
            this.timer_qpsk.Interval = 500;
            this.timer_qpsk.Tick += new System.EventHandler(this.timer_qpsk_Tick);
            // 
            // panel_txspectrum
            // 
            this.panel_txspectrum.BackColor = System.Drawing.SystemColors.ControlLight;
            this.panel_txspectrum.Location = new System.Drawing.Point(92, 590);
            this.panel_txspectrum.Name = "panel_txspectrum";
            this.panel_txspectrum.Size = new System.Drawing.Size(441, 76);
            this.panel_txspectrum.TabIndex = 6;
            this.panel_txspectrum.Paint += new System.Windows.Forms.PaintEventHandler(this.panel_txspectrum_Paint);
            // 
            // tabPage_ber
            // 
            this.tabPage_ber.Controls.Add(this.button_stopBERtest);
            this.tabPage_ber.Controls.Add(this.button_startBERtest);
            this.tabPage_ber.Controls.Add(this.rtb);
            this.tabPage_ber.ImageIndex = 4;
            this.tabPage_ber.Location = new System.Drawing.Point(4, 23);
            this.tabPage_ber.Name = "tabPage_ber";
            this.tabPage_ber.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_ber.Size = new System.Drawing.Size(1291, 552);
            this.tabPage_ber.TabIndex = 0;
            this.tabPage_ber.Text = "BER Test";
            this.tabPage_ber.UseVisualStyleBackColor = true;
            // 
            // button_stopBERtest
            // 
            this.button_stopBERtest.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button_stopBERtest.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_stopBERtest.ImageIndex = 8;
            this.button_stopBERtest.ImageList = this.imageList1;
            this.button_stopBERtest.Location = new System.Drawing.Point(113, 13);
            this.button_stopBERtest.Name = "button_stopBERtest";
            this.button_stopBERtest.Size = new System.Drawing.Size(101, 32);
            this.button_stopBERtest.TabIndex = 4;
            this.button_stopBERtest.Text = "STOP";
            this.button_stopBERtest.UseVisualStyleBackColor = true;
            this.button_stopBERtest.Click += new System.EventHandler(this.button_stopBERtest_Click);
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "binary.png");
            this.imageList1.Images.SetKeyName(1, "cancel.png");
            this.imageList1.Images.SetKeyName(2, "html.png");
            this.imageList1.Images.SetKeyName(3, "image.png");
            this.imageList1.Images.SetKeyName(4, "meter.png");
            this.imageList1.Images.SetKeyName(5, "openfolder.png");
            this.imageList1.Images.SetKeyName(6, "setup.png");
            this.imageList1.Images.SetKeyName(7, "start.png");
            this.imageList1.Images.SetKeyName(8, "stop.png");
            this.imageList1.Images.SetKeyName(9, "text.png");
            this.imageList1.Images.SetKeyName(10, "transmit.png");
            this.imageList1.Images.SetKeyName(11, "voice.png");
            this.imageList1.Images.SetKeyName(12, "about.png");
            // 
            // button_startBERtest
            // 
            this.button_startBERtest.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button_startBERtest.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_startBERtest.ImageIndex = 7;
            this.button_startBERtest.ImageList = this.imageList1;
            this.button_startBERtest.Location = new System.Drawing.Point(6, 13);
            this.button_startBERtest.Name = "button_startBERtest";
            this.button_startBERtest.Size = new System.Drawing.Size(101, 32);
            this.button_startBERtest.TabIndex = 3;
            this.button_startBERtest.Text = "START";
            this.button_startBERtest.UseVisualStyleBackColor = true;
            this.button_startBERtest.Click += new System.EventHandler(this.button_startBERtest_Click);
            // 
            // rtb
            // 
            this.rtb.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rtb.Location = new System.Drawing.Point(6, 51);
            this.rtb.Name = "rtb";
            this.rtb.Size = new System.Drawing.Size(1270, 494);
            this.rtb.TabIndex = 0;
            this.rtb.Text = "";
            // 
            // tabPage_image
            // 
            this.tabPage_image.Controls.Add(this.groupBox1);
            this.tabPage_image.Controls.Add(this.label_rximage);
            this.tabPage_image.Controls.Add(this.label_tximage);
            this.tabPage_image.Controls.Add(this.pictureBox_rximage);
            this.tabPage_image.Controls.Add(this.pictureBox_tximage);
            this.tabPage_image.ImageIndex = 3;
            this.tabPage_image.Location = new System.Drawing.Point(4, 23);
            this.tabPage_image.Name = "tabPage_image";
            this.tabPage_image.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_image.Size = new System.Drawing.Size(1291, 552);
            this.tabPage_image.TabIndex = 1;
            this.tabPage_image.Text = "Image";
            this.tabPage_image.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label_nextimage);
            this.groupBox1.Controls.Add(this.cb_loop);
            this.groupBox1.Controls.Add(this.bt_rximages);
            this.groupBox1.Controls.Add(this.button_loadimage);
            this.groupBox1.Controls.Add(this.comboBox_quality);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.checkBox_big);
            this.groupBox1.Controls.Add(this.button_cancelimg);
            this.groupBox1.Controls.Add(this.button_sendimage);
            this.groupBox1.Location = new System.Drawing.Point(3, 508);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(1277, 42);
            this.groupBox1.TabIndex = 12;
            // 
            // label_nextimage
            // 
            this.label_nextimage.AutoSize = true;
            this.label_nextimage.Location = new System.Drawing.Point(641, 19);
            this.label_nextimage.Name = "label_nextimage";
            this.label_nextimage.Size = new System.Drawing.Size(81, 13);
            this.label_nextimage.TabIndex = 12;
            this.label_nextimage.Text = "next image in ...";
            // 
            // cb_loop
            // 
            this.cb_loop.AutoSize = true;
            this.cb_loop.Location = new System.Drawing.Point(644, 5);
            this.cb_loop.Name = "cb_loop";
            this.cb_loop.Size = new System.Drawing.Size(167, 17);
            this.cb_loop.TabIndex = 11;
            this.cb_loop.Text = "loop (send all images in folder)";
            this.cb_loop.UseVisualStyleBackColor = true;
            // 
            // bt_rximages
            // 
            this.bt_rximages.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rximages.ImageIndex = 5;
            this.bt_rximages.ImageList = this.imageList1;
            this.bt_rximages.Location = new System.Drawing.Point(534, 5);
            this.bt_rximages.Name = "bt_rximages";
            this.bt_rximages.Size = new System.Drawing.Size(92, 23);
            this.bt_rximages.TabIndex = 10;
            this.bt_rximages.Text = "   RX Images";
            this.bt_rximages.UseVisualStyleBackColor = true;
            this.bt_rximages.Click += new System.EventHandler(this.bt_rximages_Click);
            // 
            // button_loadimage
            // 
            this.button_loadimage.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_loadimage.ImageIndex = 5;
            this.button_loadimage.ImageList = this.imageList1;
            this.button_loadimage.Location = new System.Drawing.Point(265, 5);
            this.button_loadimage.Name = "button_loadimage";
            this.button_loadimage.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.button_loadimage.Size = new System.Drawing.Size(88, 23);
            this.button_loadimage.TabIndex = 0;
            this.button_loadimage.Text = "    Load Image";
            this.button_loadimage.UseVisualStyleBackColor = true;
            this.button_loadimage.Click += new System.EventHandler(this.button_loadimage_Click);
            // 
            // comboBox_quality
            // 
            this.comboBox_quality.FormattingEnabled = true;
            this.comboBox_quality.Items.AddRange(new object[] {
            "low, 30s",
            "medium, 1min",
            "high, 2min",
            "very high, 4min"});
            this.comboBox_quality.Location = new System.Drawing.Point(57, 7);
            this.comboBox_quality.Name = "comboBox_quality";
            this.comboBox_quality.Size = new System.Drawing.Size(109, 21);
            this.comboBox_quality.TabIndex = 6;
            this.comboBox_quality.Text = "medium, 1min";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(8, 10);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(42, 13);
            this.label2.TabIndex = 7;
            this.label2.Text = "Quality:";
            // 
            // checkBox_big
            // 
            this.checkBox_big.AutoSize = true;
            this.checkBox_big.Checked = true;
            this.checkBox_big.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox_big.Location = new System.Drawing.Point(187, 9);
            this.checkBox_big.Name = "checkBox_big";
            this.checkBox_big.Size = new System.Drawing.Size(79, 17);
            this.checkBox_big.TabIndex = 8;
            this.checkBox_big.Text = "640(320)px";
            this.checkBox_big.UseVisualStyleBackColor = true;
            this.checkBox_big.CheckedChanged += new System.EventHandler(this.checkBox_small_CheckedChanged);
            // 
            // button_cancelimg
            // 
            this.button_cancelimg.ForeColor = System.Drawing.Color.Red;
            this.button_cancelimg.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_cancelimg.ImageIndex = 8;
            this.button_cancelimg.ImageList = this.imageList1;
            this.button_cancelimg.Location = new System.Drawing.Point(443, 5);
            this.button_cancelimg.Name = "button_cancelimg";
            this.button_cancelimg.Size = new System.Drawing.Size(75, 23);
            this.button_cancelimg.TabIndex = 9;
            this.button_cancelimg.Text = "   Cancel";
            this.button_cancelimg.UseVisualStyleBackColor = true;
            this.button_cancelimg.Click += new System.EventHandler(this.button_cancelimg_Click);
            // 
            // button_sendimage
            // 
            this.button_sendimage.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button_sendimage.ForeColor = System.Drawing.Color.Blue;
            this.button_sendimage.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button_sendimage.ImageIndex = 10;
            this.button_sendimage.ImageList = this.imageList1;
            this.button_sendimage.Location = new System.Drawing.Point(361, 5);
            this.button_sendimage.Name = "button_sendimage";
            this.button_sendimage.Size = new System.Drawing.Size(75, 23);
            this.button_sendimage.TabIndex = 1;
            this.button_sendimage.Text = "Send";
            this.button_sendimage.UseVisualStyleBackColor = true;
            this.button_sendimage.Click += new System.EventHandler(this.button_sendimage_Click);
            // 
            // label_rximage
            // 
            this.label_rximage.AutoSize = true;
            this.label_rximage.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_rximage.Location = new System.Drawing.Point(648, 7);
            this.label_rximage.Name = "label_rximage";
            this.label_rximage.Size = new System.Drawing.Size(61, 13);
            this.label_rximage.TabIndex = 5;
            this.label_rximage.Text = "RX image";
            // 
            // label_tximage
            // 
            this.label_tximage.AutoSize = true;
            this.label_tximage.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_tximage.Location = new System.Drawing.Point(6, 7);
            this.label_tximage.Name = "label_tximage";
            this.label_tximage.Size = new System.Drawing.Size(60, 13);
            this.label_tximage.TabIndex = 4;
            this.label_tximage.Text = "TX image";
            // 
            // pictureBox_rximage
            // 
            this.pictureBox_rximage.BackgroundImage = global::oscardata.Properties.Resources.hintergrundxcf;
            this.pictureBox_rximage.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBox_rximage.Location = new System.Drawing.Point(642, 27);
            this.pictureBox_rximage.Name = "pictureBox_rximage";
            this.pictureBox_rximage.Size = new System.Drawing.Size(640, 480);
            this.pictureBox_rximage.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox_rximage.TabIndex = 3;
            this.pictureBox_rximage.TabStop = false;
            // 
            // pictureBox_tximage
            // 
            this.pictureBox_tximage.BackgroundImage = global::oscardata.Properties.Resources.hintergrundxcf;
            this.pictureBox_tximage.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.pictureBox_tximage.Location = new System.Drawing.Point(1, 27);
            this.pictureBox_tximage.Name = "pictureBox_tximage";
            this.pictureBox_tximage.Size = new System.Drawing.Size(640, 480);
            this.pictureBox_tximage.TabIndex = 2;
            this.pictureBox_tximage.TabStop = false;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage_image);
            this.tabControl1.Controls.Add(this.tabPage_file);
            this.tabControl1.Controls.Add(this.tabPage_audio);
            this.tabControl1.Controls.Add(this.tabPage_ber);
            this.tabControl1.Controls.Add(this.tabPage_setup);
            this.tabControl1.Controls.Add(this.tabPage_about);
            this.tabControl1.ImageList = this.imageList1;
            this.tabControl1.Location = new System.Drawing.Point(5, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1299, 579);
            this.tabControl1.TabIndex = 3;
            // 
            // tabPage_file
            // 
            this.tabPage_file.Controls.Add(this.pictureBox1);
            this.tabPage_file.Controls.Add(this.button2);
            this.tabPage_file.Controls.Add(this.bt_openrxfile);
            this.tabPage_file.Controls.Add(this.label_rxfile);
            this.tabPage_file.Controls.Add(this.label_txfile);
            this.tabPage_file.Controls.Add(this.rtb_RXfile);
            this.tabPage_file.Controls.Add(this.rtb_TXfile);
            this.tabPage_file.Controls.Add(this.bt_file_send);
            this.tabPage_file.Controls.Add(this.bt_sendBinaryFile);
            this.tabPage_file.Controls.Add(this.bt_file_html);
            this.tabPage_file.Controls.Add(this.bt_file_ascii);
            this.tabPage_file.ImageIndex = 9;
            this.tabPage_file.Location = new System.Drawing.Point(4, 23);
            this.tabPage_file.Name = "tabPage_file";
            this.tabPage_file.Size = new System.Drawing.Size(1291, 552);
            this.tabPage_file.TabIndex = 2;
            this.tabPage_file.Text = "File";
            this.tabPage_file.UseVisualStyleBackColor = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImage = global::oscardata.Properties.Resources.text_big;
            this.pictureBox1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.pictureBox1.Location = new System.Drawing.Point(17, 371);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(127, 134);
            this.pictureBox1.TabIndex = 13;
            this.pictureBox1.TabStop = false;
            // 
            // button2
            // 
            this.button2.ForeColor = System.Drawing.Color.Red;
            this.button2.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button2.ImageIndex = 8;
            this.button2.ImageList = this.imageList1;
            this.button2.Location = new System.Drawing.Point(17, 218);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(137, 23);
            this.button2.TabIndex = 12;
            this.button2.Text = "Cancel";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button_cancelimg_Click);
            // 
            // bt_openrxfile
            // 
            this.bt_openrxfile.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_openrxfile.ImageIndex = 5;
            this.bt_openrxfile.ImageList = this.imageList1;
            this.bt_openrxfile.Location = new System.Drawing.Point(17, 306);
            this.bt_openrxfile.Name = "bt_openrxfile";
            this.bt_openrxfile.Size = new System.Drawing.Size(137, 30);
            this.bt_openrxfile.TabIndex = 11;
            this.bt_openrxfile.Text = "Open RX file folder";
            this.bt_openrxfile.UseVisualStyleBackColor = true;
            this.bt_openrxfile.Click += new System.EventHandler(this.bt_openrxfile_Click);
            // 
            // label_rxfile
            // 
            this.label_rxfile.AutoSize = true;
            this.label_rxfile.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_rxfile.Location = new System.Drawing.Point(749, 10);
            this.label_rxfile.Name = "label_rxfile";
            this.label_rxfile.Size = new System.Drawing.Size(48, 13);
            this.label_rxfile.TabIndex = 7;
            this.label_rxfile.Text = "RX File";
            // 
            // label_txfile
            // 
            this.label_txfile.AutoSize = true;
            this.label_txfile.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_txfile.Location = new System.Drawing.Point(209, 10);
            this.label_txfile.Name = "label_txfile";
            this.label_txfile.Size = new System.Drawing.Size(47, 13);
            this.label_txfile.TabIndex = 6;
            this.label_txfile.Text = "TX File";
            // 
            // rtb_RXfile
            // 
            this.rtb_RXfile.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rtb_RXfile.Location = new System.Drawing.Point(736, 31);
            this.rtb_RXfile.Name = "rtb_RXfile";
            this.rtb_RXfile.Size = new System.Drawing.Size(526, 508);
            this.rtb_RXfile.TabIndex = 5;
            this.rtb_RXfile.Text = "";
            // 
            // rtb_TXfile
            // 
            this.rtb_TXfile.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rtb_TXfile.Location = new System.Drawing.Point(204, 31);
            this.rtb_TXfile.Name = "rtb_TXfile";
            this.rtb_TXfile.Size = new System.Drawing.Size(526, 508);
            this.rtb_TXfile.TabIndex = 4;
            this.rtb_TXfile.Text = "";
            // 
            // bt_file_send
            // 
            this.bt_file_send.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bt_file_send.ForeColor = System.Drawing.Color.Blue;
            this.bt_file_send.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_file_send.ImageIndex = 10;
            this.bt_file_send.ImageList = this.imageList1;
            this.bt_file_send.Location = new System.Drawing.Point(17, 157);
            this.bt_file_send.Name = "bt_file_send";
            this.bt_file_send.Size = new System.Drawing.Size(137, 30);
            this.bt_file_send.TabIndex = 3;
            this.bt_file_send.Text = "SEND";
            this.bt_file_send.UseVisualStyleBackColor = true;
            this.bt_file_send.Click += new System.EventHandler(this.bt_file_send_Click);
            // 
            // bt_sendBinaryFile
            // 
            this.bt_sendBinaryFile.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_sendBinaryFile.ImageIndex = 0;
            this.bt_sendBinaryFile.ImageList = this.imageList1;
            this.bt_sendBinaryFile.Location = new System.Drawing.Point(17, 89);
            this.bt_sendBinaryFile.Name = "bt_sendBinaryFile";
            this.bt_sendBinaryFile.Size = new System.Drawing.Size(137, 23);
            this.bt_sendBinaryFile.TabIndex = 2;
            this.bt_sendBinaryFile.Text = "Load Binary File";
            this.bt_sendBinaryFile.UseVisualStyleBackColor = true;
            this.bt_sendBinaryFile.Click += new System.EventHandler(this.bt_sendBinaryFile_Click);
            // 
            // bt_file_html
            // 
            this.bt_file_html.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_file_html.ImageIndex = 2;
            this.bt_file_html.ImageList = this.imageList1;
            this.bt_file_html.Location = new System.Drawing.Point(17, 60);
            this.bt_file_html.Name = "bt_file_html";
            this.bt_file_html.Size = new System.Drawing.Size(137, 23);
            this.bt_file_html.TabIndex = 1;
            this.bt_file_html.Text = "Load HTML File";
            this.bt_file_html.UseVisualStyleBackColor = true;
            this.bt_file_html.Click += new System.EventHandler(this.button2_Click);
            // 
            // bt_file_ascii
            // 
            this.bt_file_ascii.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_file_ascii.ImageIndex = 9;
            this.bt_file_ascii.ImageList = this.imageList1;
            this.bt_file_ascii.Location = new System.Drawing.Point(17, 31);
            this.bt_file_ascii.Name = "bt_file_ascii";
            this.bt_file_ascii.Size = new System.Drawing.Size(137, 23);
            this.bt_file_ascii.TabIndex = 0;
            this.bt_file_ascii.Text = "   Load ASCII Text File";
            this.bt_file_ascii.UseVisualStyleBackColor = true;
            this.bt_file_ascii.Click += new System.EventHandler(this.bt_file_ascii_Click);
            // 
            // tabPage_audio
            // 
            this.tabPage_audio.Controls.Add(this.groupBox7);
            this.tabPage_audio.Controls.Add(this.groupBox6);
            this.tabPage_audio.Controls.Add(this.groupBox5);
            this.tabPage_audio.ImageIndex = 11;
            this.tabPage_audio.Location = new System.Drawing.Point(4, 23);
            this.tabPage_audio.Name = "tabPage_audio";
            this.tabPage_audio.Size = new System.Drawing.Size(1291, 552);
            this.tabPage_audio.TabIndex = 5;
            this.tabPage_audio.Text = "Voice Audio";
            this.tabPage_audio.UseVisualStyleBackColor = true;
            // 
            // groupBox7
            // 
            this.groupBox7.Controls.Add(this.rb_codec2);
            this.groupBox7.Controls.Add(this.pictureBox2);
            this.groupBox7.Controls.Add(this.rb_opus);
            this.groupBox7.Location = new System.Drawing.Point(18, 295);
            this.groupBox7.Name = "groupBox7";
            this.groupBox7.Size = new System.Drawing.Size(765, 95);
            this.groupBox7.TabIndex = 21;
            this.groupBox7.TabStop = false;
            this.groupBox7.Text = "Codec Selection";
            // 
            // rb_codec2
            // 
            this.rb_codec2.AutoSize = true;
            this.rb_codec2.Location = new System.Drawing.Point(92, 50);
            this.rb_codec2.Name = "rb_codec2";
            this.rb_codec2.Size = new System.Drawing.Size(342, 17);
            this.rb_codec2.TabIndex = 23;
            this.rb_codec2.Text = "CODEC-2 parametric audio codec. For QPSK. Audio rate: 3200 bps";
            this.rb_codec2.UseVisualStyleBackColor = true;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackgroundImage = global::oscardata.Properties.Resources.voice_big;
            this.pictureBox2.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox2.Location = new System.Drawing.Point(6, 13);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(75, 76);
            this.pictureBox2.TabIndex = 22;
            this.pictureBox2.TabStop = false;
            // 
            // rb_opus
            // 
            this.rb_opus.AutoSize = true;
            this.rb_opus.Checked = true;
            this.rb_opus.Location = new System.Drawing.Point(92, 27);
            this.rb_opus.Name = "rb_opus";
            this.rb_opus.Size = new System.Drawing.Size(348, 17);
            this.rb_opus.TabIndex = 0;
            this.rb_opus.TabStop = true;
            this.rb_opus.Text = "OPUS rate adaptive codec. For 8APSK. Audio rate: 84% of data rate";
            this.rb_opus.UseVisualStyleBackColor = true;
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.pb_voice);
            this.groupBox6.Controls.Add(this.cb_digitalVoiceRXonly);
            this.groupBox6.Controls.Add(this.cb_digitalVoice);
            this.groupBox6.Controls.Add(this.cb_codecloop);
            this.groupBox6.Controls.Add(this.cb_switchtoLS);
            this.groupBox6.Controls.Add(this.cb_voiceloop);
            this.groupBox6.Location = new System.Drawing.Point(18, 135);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(765, 154);
            this.groupBox6.TabIndex = 20;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Voice Audio Operating Mode";
            // 
            // pb_voice
            // 
            this.pb_voice.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.pb_voice.Location = new System.Drawing.Point(308, 19);
            this.pb_voice.Name = "pb_voice";
            this.pb_voice.Size = new System.Drawing.Size(440, 85);
            this.pb_voice.TabIndex = 22;
            this.pb_voice.TabStop = false;
            // 
            // cb_digitalVoiceRXonly
            // 
            this.cb_digitalVoiceRXonly.AutoSize = true;
            this.cb_digitalVoiceRXonly.Location = new System.Drawing.Point(15, 100);
            this.cb_digitalVoiceRXonly.Name = "cb_digitalVoiceRXonly";
            this.cb_digitalVoiceRXonly.Size = new System.Drawing.Size(308, 17);
            this.cb_digitalVoiceRXonly.TabIndex = 21;
            this.cb_digitalVoiceRXonly.Text = "Digital Voice RX:        Receiver ---> Codec ---> Loudspeaker";
            this.cb_digitalVoiceRXonly.UseVisualStyleBackColor = true;
            this.cb_digitalVoiceRXonly.CheckedChanged += new System.EventHandler(this.cb_digitalVoiceRXonly_CheckedChanged);
            // 
            // cb_digitalVoice
            // 
            this.cb_digitalVoice.AutoSize = true;
            this.cb_digitalVoice.Location = new System.Drawing.Point(15, 123);
            this.cb_digitalVoice.Name = "cb_digitalVoice";
            this.cb_digitalVoice.Size = new System.Drawing.Size(508, 17);
            this.cb_digitalVoice.TabIndex = 20;
            this.cb_digitalVoice.Text = "Digital Voice RX+TX: Microphone ---> Codec ---> Transmitter   |   Receiver ---> C" +
    "odec ---> Loudspeaker";
            this.cb_digitalVoice.UseVisualStyleBackColor = true;
            this.cb_digitalVoice.CheckedChanged += new System.EventHandler(this.cb_digitalVoice_CheckedChanged);
            // 
            // cb_codecloop
            // 
            this.cb_codecloop.AutoSize = true;
            this.cb_codecloop.Location = new System.Drawing.Point(15, 77);
            this.cb_codecloop.Name = "cb_codecloop";
            this.cb_codecloop.Size = new System.Drawing.Size(284, 17);
            this.cb_codecloop.TabIndex = 19;
            this.cb_codecloop.Text = "Codec Loop:  Microphone ---> Codec ---> Loudspeaker";
            this.cb_codecloop.UseVisualStyleBackColor = true;
            this.cb_codecloop.CheckedChanged += new System.EventHandler(this.cb_codecloop_CheckedChanged);
            // 
            // cb_switchtoLS
            // 
            this.cb_switchtoLS.AutoSize = true;
            this.cb_switchtoLS.Location = new System.Drawing.Point(15, 31);
            this.cb_switchtoLS.Name = "cb_switchtoLS";
            this.cb_switchtoLS.Size = new System.Drawing.Size(225, 17);
            this.cb_switchtoLS.TabIndex = 17;
            this.cb_switchtoLS.Text = "Digital Monitor: Receiver ---> Loudspeaker";
            this.cb_switchtoLS.UseVisualStyleBackColor = true;
            this.cb_switchtoLS.CheckedChanged += new System.EventHandler(this.cb_switchtoLS_CheckedChanged);
            // 
            // cb_voiceloop
            // 
            this.cb_voiceloop.AutoSize = true;
            this.cb_voiceloop.Location = new System.Drawing.Point(15, 54);
            this.cb_voiceloop.Name = "cb_voiceloop";
            this.cb_voiceloop.Size = new System.Drawing.Size(232, 17);
            this.cb_voiceloop.TabIndex = 18;
            this.cb_voiceloop.Text = "internal Loop: Microphone ---> Loudspeaker";
            this.cb_voiceloop.UseVisualStyleBackColor = true;
            this.cb_voiceloop.CheckedChanged += new System.EventHandler(this.cb_voiceloop_CheckedChanged);
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.pb_voiceCAPstatus);
            this.groupBox5.Controls.Add(this.pb_voicePBstatus);
            this.groupBox5.Controls.Add(this.label8);
            this.groupBox5.Controls.Add(this.label9);
            this.groupBox5.Controls.Add(this.tb_mic);
            this.groupBox5.Controls.Add(this.tb_loadspeaker);
            this.groupBox5.Controls.Add(this.cb_loudspeaker);
            this.groupBox5.Controls.Add(this.label10);
            this.groupBox5.Controls.Add(this.cb_mic);
            this.groupBox5.Controls.Add(this.label11);
            this.groupBox5.Location = new System.Drawing.Point(18, 21);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(765, 107);
            this.groupBox5.TabIndex = 15;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "Loadspeaker / Microphone / Headset";
            // 
            // pb_voiceCAPstatus
            // 
            this.pb_voiceCAPstatus.BackgroundImage = global::oscardata.Properties.Resources.ok;
            this.pb_voiceCAPstatus.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_voiceCAPstatus.Location = new System.Drawing.Point(387, 58);
            this.pb_voiceCAPstatus.Name = "pb_voiceCAPstatus";
            this.pb_voiceCAPstatus.Size = new System.Drawing.Size(21, 21);
            this.pb_voiceCAPstatus.TabIndex = 19;
            this.pb_voiceCAPstatus.TabStop = false;
            // 
            // pb_voicePBstatus
            // 
            this.pb_voicePBstatus.BackgroundImage = global::oscardata.Properties.Resources.ok;
            this.pb_voicePBstatus.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_voicePBstatus.Location = new System.Drawing.Point(387, 25);
            this.pb_voicePBstatus.Name = "pb_voicePBstatus";
            this.pb_voicePBstatus.Size = new System.Drawing.Size(21, 21);
            this.pb_voicePBstatus.TabIndex = 18;
            this.pb_voicePBstatus.TabStop = false;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(423, 60);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(45, 13);
            this.label8.TabIndex = 16;
            this.label8.Text = "Volume:";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(423, 29);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(45, 13);
            this.label9.TabIndex = 15;
            this.label9.Text = "Volume:";
            // 
            // tb_mic
            // 
            this.tb_mic.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.tb_mic.Location = new System.Drawing.Point(474, 56);
            this.tb_mic.Maximum = 100;
            this.tb_mic.MaximumSize = new System.Drawing.Size(0, 24);
            this.tb_mic.MinimumSize = new System.Drawing.Size(100, 0);
            this.tb_mic.Name = "tb_mic";
            this.tb_mic.Size = new System.Drawing.Size(100, 24);
            this.tb_mic.TabIndex = 14;
            this.tb_mic.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tb_mic.Value = 50;
            this.tb_mic.Scroll += new System.EventHandler(this.tb_mic_Scroll);
            // 
            // tb_loadspeaker
            // 
            this.tb_loadspeaker.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.tb_loadspeaker.Location = new System.Drawing.Point(474, 25);
            this.tb_loadspeaker.Maximum = 100;
            this.tb_loadspeaker.MaximumSize = new System.Drawing.Size(0, 24);
            this.tb_loadspeaker.MinimumSize = new System.Drawing.Size(100, 0);
            this.tb_loadspeaker.Name = "tb_loadspeaker";
            this.tb_loadspeaker.Size = new System.Drawing.Size(100, 24);
            this.tb_loadspeaker.TabIndex = 13;
            this.tb_loadspeaker.TickStyle = System.Windows.Forms.TickStyle.None;
            this.tb_loadspeaker.Value = 50;
            this.tb_loadspeaker.Scroll += new System.EventHandler(this.tb_loadspeaker_Scroll);
            // 
            // cb_loudspeaker
            // 
            this.cb_loudspeaker.FormattingEnabled = true;
            this.cb_loudspeaker.Location = new System.Drawing.Point(151, 25);
            this.cb_loudspeaker.Name = "cb_loudspeaker";
            this.cb_loudspeaker.Size = new System.Drawing.Size(230, 21);
            this.cb_loudspeaker.TabIndex = 7;
            this.cb_loudspeaker.Text = "Default";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Location = new System.Drawing.Point(12, 29);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(133, 13);
            this.label10.TabIndex = 8;
            this.label10.Text = "Loadspeaker/Headphone:";
            // 
            // cb_mic
            // 
            this.cb_mic.FormattingEnabled = true;
            this.cb_mic.Location = new System.Drawing.Point(151, 58);
            this.cb_mic.Name = "cb_mic";
            this.cb_mic.Size = new System.Drawing.Size(230, 21);
            this.cb_mic.TabIndex = 9;
            this.cb_mic.Text = "Default";
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(12, 62);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(66, 13);
            this.label11.TabIndex = 10;
            this.label11.Text = "Microphone:";
            // 
            // tabPage_setup
            // 
            this.tabPage_setup.Controls.Add(this.groupBox4);
            this.tabPage_setup.Controls.Add(this.groupBox3);
            this.tabPage_setup.Controls.Add(this.groupBox2);
            this.tabPage_setup.ImageIndex = 6;
            this.tabPage_setup.Location = new System.Drawing.Point(4, 23);
            this.tabPage_setup.Name = "tabPage_setup";
            this.tabPage_setup.Size = new System.Drawing.Size(1291, 552);
            this.tabPage_setup.TabIndex = 4;
            this.tabPage_setup.Text = "Setup";
            this.tabPage_setup.UseVisualStyleBackColor = true;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.cb_language);
            this.groupBox4.Controls.Add(this.cb_autostart);
            this.groupBox4.Controls.Add(this.bt_shutdown);
            this.groupBox4.Controls.Add(this.tb_shutdown);
            this.groupBox4.Controls.Add(this.bt_resetmodem);
            this.groupBox4.Controls.Add(this.textBox3);
            this.groupBox4.Location = new System.Drawing.Point(12, 259);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(674, 121);
            this.groupBox4.TabIndex = 15;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Maintenance";
            // 
            // cb_language
            // 
            this.cb_language.FormattingEnabled = true;
            this.cb_language.Items.AddRange(new object[] {
            "English",
            "German/Deutsch"});
            this.cb_language.Location = new System.Drawing.Point(207, 19);
            this.cb_language.Name = "cb_language";
            this.cb_language.Size = new System.Drawing.Size(110, 21);
            this.cb_language.TabIndex = 24;
            this.cb_language.Text = "English";
            this.cb_language.SelectedIndexChanged += new System.EventHandler(this.cb_language_SelectedIndexChanged);
            // 
            // cb_autostart
            // 
            this.cb_autostart.AutoSize = true;
            this.cb_autostart.Checked = true;
            this.cb_autostart.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cb_autostart.Location = new System.Drawing.Point(17, 23);
            this.cb_autostart.Name = "cb_autostart";
            this.cb_autostart.Size = new System.Drawing.Size(156, 17);
            this.cb_autostart.TabIndex = 4;
            this.cb_autostart.Text = "AUTO start/stop HSmodem";
            this.cb_autostart.UseVisualStyleBackColor = true;
            this.cb_autostart.CheckedChanged += new System.EventHandler(this.cb_autostart_CheckedChanged);
            // 
            // bt_shutdown
            // 
            this.bt_shutdown.Location = new System.Drawing.Point(491, 19);
            this.bt_shutdown.Name = "bt_shutdown";
            this.bt_shutdown.Size = new System.Drawing.Size(155, 23);
            this.bt_shutdown.TabIndex = 4;
            this.bt_shutdown.Text = "Shutdown Modem-SBC";
            this.bt_shutdown.UseVisualStyleBackColor = true;
            this.bt_shutdown.Click += new System.EventHandler(this.bt_shutdown_Click);
            // 
            // tb_shutdown
            // 
            this.tb_shutdown.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb_shutdown.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_shutdown.ForeColor = System.Drawing.Color.Red;
            this.tb_shutdown.Location = new System.Drawing.Point(491, 48);
            this.tb_shutdown.Multiline = true;
            this.tb_shutdown.Name = "tb_shutdown";
            this.tb_shutdown.Size = new System.Drawing.Size(155, 50);
            this.tb_shutdown.TabIndex = 5;
            this.tb_shutdown.Text = "before switching off the \r\nmodem SBC click here to \r\navoid defective SD-cards.\r\n";
            // 
            // bt_resetmodem
            // 
            this.bt_resetmodem.Location = new System.Drawing.Point(356, 82);
            this.bt_resetmodem.Name = "bt_resetmodem";
            this.bt_resetmodem.Size = new System.Drawing.Size(117, 23);
            this.bt_resetmodem.TabIndex = 6;
            this.bt_resetmodem.Text = "Reset RX Modem";
            this.bt_resetmodem.UseVisualStyleBackColor = true;
            this.bt_resetmodem.Visible = false;
            this.bt_resetmodem.Click += new System.EventHandler(this.bt_resetmodem_Click);
            // 
            // textBox3
            // 
            this.textBox3.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox3.ForeColor = System.Drawing.Color.Black;
            this.textBox3.Location = new System.Drawing.Point(15, 46);
            this.textBox3.Multiline = true;
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(151, 50);
            this.textBox3.TabIndex = 12;
            this.textBox3.Text = "only uncheck if modem runs on a separate PC";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.pb_audioCAPstatus);
            this.groupBox3.Controls.Add(this.pb_audioPBstatus);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.tb_CAPvol);
            this.groupBox3.Controls.Add(this.tb_PBvol);
            this.groupBox3.Controls.Add(this.cb_audioPB);
            this.groupBox3.Controls.Add(this.label3);
            this.groupBox3.Controls.Add(this.cb_audioCAP);
            this.groupBox3.Controls.Add(this.label4);
            this.groupBox3.Location = new System.Drawing.Point(12, 146);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(674, 107);
            this.groupBox3.TabIndex = 14;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Transceiver Audio";
            // 
            // pb_audioCAPstatus
            // 
            this.pb_audioCAPstatus.BackgroundImage = global::oscardata.Properties.Resources.ok;
            this.pb_audioCAPstatus.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_audioCAPstatus.Location = new System.Drawing.Point(375, 52);
            this.pb_audioCAPstatus.Name = "pb_audioCAPstatus";
            this.pb_audioCAPstatus.Size = new System.Drawing.Size(21, 21);
            this.pb_audioCAPstatus.TabIndex = 18;
            this.pb_audioCAPstatus.TabStop = false;
            // 
            // pb_audioPBstatus
            // 
            this.pb_audioPBstatus.BackgroundImage = global::oscardata.Properties.Resources.ok;
            this.pb_audioPBstatus.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_audioPBstatus.Location = new System.Drawing.Point(375, 19);
            this.pb_audioPBstatus.Name = "pb_audioPBstatus";
            this.pb_audioPBstatus.Size = new System.Drawing.Size(21, 21);
            this.pb_audioPBstatus.TabIndex = 17;
            this.pb_audioPBstatus.TabStop = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(428, 54);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(45, 13);
            this.label6.TabIndex = 16;
            this.label6.Text = "Volume:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(428, 23);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(45, 13);
            this.label5.TabIndex = 15;
            this.label5.Text = "Volume:";
            // 
            // tb_CAPvol
            // 
            this.tb_CAPvol.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.tb_CAPvol.Location = new System.Drawing.Point(479, 50);
            this.tb_CAPvol.Maximum = 100;
            this.tb_CAPvol.MaximumSize = new System.Drawing.Size(0, 24);
            this.tb_CAPvol.MinimumSize = new System.Drawing.Size(100, 0);
            this.tb_CAPvol.Name = "tb_CAPvol";
            this.tb_CAPvol.Size = new System.Drawing.Size(100, 24);
            this.tb_CAPvol.TabIndex = 14;
            this.tb_CAPvol.TickFrequency = 10;
            this.tb_CAPvol.Value = 50;
            this.tb_CAPvol.Scroll += new System.EventHandler(this.tb_CAPvol_Scroll);
            // 
            // tb_PBvol
            // 
            this.tb_PBvol.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.tb_PBvol.Location = new System.Drawing.Point(479, 19);
            this.tb_PBvol.Maximum = 100;
            this.tb_PBvol.MaximumSize = new System.Drawing.Size(0, 24);
            this.tb_PBvol.MinimumSize = new System.Drawing.Size(100, 0);
            this.tb_PBvol.Name = "tb_PBvol";
            this.tb_PBvol.Size = new System.Drawing.Size(100, 24);
            this.tb_PBvol.TabIndex = 13;
            this.tb_PBvol.TickFrequency = 10;
            this.tb_PBvol.Value = 50;
            this.tb_PBvol.Scroll += new System.EventHandler(this.tb_PBvol_Scroll);
            // 
            // cb_audioPB
            // 
            this.cb_audioPB.FormattingEnabled = true;
            this.cb_audioPB.Location = new System.Drawing.Point(138, 19);
            this.cb_audioPB.Name = "cb_audioPB";
            this.cb_audioPB.Size = new System.Drawing.Size(230, 21);
            this.cb_audioPB.TabIndex = 7;
            this.cb_audioPB.Text = "Default";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 23);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(121, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Audio Playback Device:";
            // 
            // cb_audioCAP
            // 
            this.cb_audioCAP.FormattingEnabled = true;
            this.cb_audioCAP.Location = new System.Drawing.Point(138, 52);
            this.cb_audioCAP.Name = "cb_audioCAP";
            this.cb_audioCAP.Size = new System.Drawing.Size(230, 21);
            this.cb_audioCAP.TabIndex = 9;
            this.cb_audioCAP.Text = "Default";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 56);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(112, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Audio Record Device:";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.cb_stampinfo);
            this.groupBox2.Controls.Add(this.tb_info);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.textBox5);
            this.groupBox2.Controls.Add(this.cb_announcement);
            this.groupBox2.Controls.Add(this.textBox4);
            this.groupBox2.Controls.Add(this.textBox1);
            this.groupBox2.Controls.Add(this.tb_callsign);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.cb_stampcall);
            this.groupBox2.Location = new System.Drawing.Point(12, 13);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(674, 126);
            this.groupBox2.TabIndex = 13;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Personal Settings";
            // 
            // cb_stampinfo
            // 
            this.cb_stampinfo.AutoSize = true;
            this.cb_stampinfo.Checked = true;
            this.cb_stampinfo.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cb_stampinfo.Location = new System.Drawing.Point(71, 82);
            this.cb_stampinfo.Name = "cb_stampinfo";
            this.cb_stampinfo.Size = new System.Drawing.Size(128, 17);
            this.cb_stampinfo.TabIndex = 23;
            this.cb_stampinfo.Text = "Insert Info into picture";
            this.cb_stampinfo.UseVisualStyleBackColor = true;
            // 
            // tb_info
            // 
            this.tb_info.Location = new System.Drawing.Point(243, 28);
            this.tb_info.Name = "tb_info";
            this.tb_info.Size = new System.Drawing.Size(413, 20);
            this.tb_info.TabIndex = 22;
            this.tb_info.Text = "tnx fer QSO, vy 73";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(204, 31);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(28, 13);
            this.label7.TabIndex = 21;
            this.label7.Text = "Info:";
            // 
            // textBox5
            // 
            this.textBox5.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox5.ForeColor = System.Drawing.Color.Black;
            this.textBox5.Location = new System.Drawing.Point(261, 92);
            this.textBox5.Multiline = true;
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(34, 19);
            this.textBox5.TabIndex = 20;
            this.textBox5.Text = "every";
            // 
            // cb_announcement
            // 
            this.cb_announcement.FormattingEnabled = true;
            this.cb_announcement.Items.AddRange(new object[] {
            "off",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10"});
            this.cb_announcement.Location = new System.Drawing.Point(296, 90);
            this.cb_announcement.Name = "cb_announcement";
            this.cb_announcement.Size = new System.Drawing.Size(56, 21);
            this.cb_announcement.TabIndex = 19;
            this.cb_announcement.Text = "4";
            // 
            // textBox4
            // 
            this.textBox4.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox4.ForeColor = System.Drawing.Color.Black;
            this.textBox4.Location = new System.Drawing.Point(361, 92);
            this.textBox4.Multiline = true;
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(75, 19);
            this.textBox4.TabIndex = 18;
            this.textBox4.Text = "transmissions";
            // 
            // textBox1
            // 
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox1.ForeColor = System.Drawing.Color.Black;
            this.textBox1.Location = new System.Drawing.Point(259, 68);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(253, 19);
            this.textBox1.TabIndex = 17;
            this.textBox1.Text = "send announcement before pic/file transfer";
            // 
            // tb_callsign
            // 
            this.tb_callsign.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.tb_callsign.Location = new System.Drawing.Point(71, 28);
            this.tb_callsign.Name = "tb_callsign";
            this.tb_callsign.Size = new System.Drawing.Size(104, 20);
            this.tb_callsign.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 31);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(46, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Callsign:";
            // 
            // cb_stampcall
            // 
            this.cb_stampcall.AutoSize = true;
            this.cb_stampcall.Checked = true;
            this.cb_stampcall.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cb_stampcall.Location = new System.Drawing.Point(71, 64);
            this.cb_stampcall.Name = "cb_stampcall";
            this.cb_stampcall.Size = new System.Drawing.Size(146, 17);
            this.cb_stampcall.TabIndex = 2;
            this.cb_stampcall.Text = "Insert Callsign into picture";
            this.cb_stampcall.UseVisualStyleBackColor = true;
            // 
            // tabPage_about
            // 
            this.tabPage_about.Controls.Add(this.richTextBox1);
            this.tabPage_about.ImageIndex = 12;
            this.tabPage_about.Location = new System.Drawing.Point(4, 23);
            this.tabPage_about.Name = "tabPage_about";
            this.tabPage_about.Size = new System.Drawing.Size(1291, 552);
            this.tabPage_about.TabIndex = 6;
            this.tabPage_about.Text = "About";
            this.tabPage_about.UseVisualStyleBackColor = true;
            // 
            // richTextBox1
            // 
            this.richTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.richTextBox1.Location = new System.Drawing.Point(17, 22);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(855, 527);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = resources.GetString("richTextBox1.Text");
            // 
            // cb_speed
            // 
            this.cb_speed.FormattingEnabled = true;
            this.cb_speed.Items.AddRange(new object[] {
            "3000 QPSK BW: 1700 Hz ",
            "4000 QPSK BW: 2400 Hz ",
            "4410 QPSK BW: 2500 Hz (QO-100 Standard)",
            "4800 QPSK BW: 2700 Hz",
            "5500 8APSK BW: 2300 Hz",
            "6000 8APSK BW: 2500 Hz (QO-100 Transceiver)",
            "6600 8APSK BW: 2600 Hz",
            "7200 8APSK BW: 2700 Hz (QO-100 SDR)"});
            this.cb_speed.Location = new System.Drawing.Point(658, 591);
            this.cb_speed.Name = "cb_speed";
            this.cb_speed.Size = new System.Drawing.Size(304, 21);
            this.cb_speed.TabIndex = 11;
            this.cb_speed.Text = "4410 QPSK BW: 2500 Hz (QO-100)";
            this.cb_speed.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // label_speed
            // 
            this.label_speed.AutoSize = true;
            this.label_speed.Location = new System.Drawing.Point(567, 594);
            this.label_speed.Name = "label_speed";
            this.label_speed.Size = new System.Drawing.Size(71, 13);
            this.label_speed.TabIndex = 12;
            this.label_speed.Text = "Speed [bit/s]:";
            // 
            // timer_searchmodem
            // 
            this.timer_searchmodem.Interval = 1000;
            this.timer_searchmodem.Tick += new System.EventHandler(this.timer_searchmodem_Tick);
            // 
            // progressBar_fifo
            // 
            this.progressBar_fifo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.progressBar_fifo.Location = new System.Drawing.Point(658, 618);
            this.progressBar_fifo.Maximum = 20;
            this.progressBar_fifo.Name = "progressBar_fifo";
            this.progressBar_fifo.Size = new System.Drawing.Size(304, 18);
            this.progressBar_fifo.Step = 11;
            this.progressBar_fifo.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar_fifo.TabIndex = 13;
            // 
            // label_fifo
            // 
            this.label_fifo.AutoSize = true;
            this.label_fifo.Location = new System.Drawing.Point(567, 620);
            this.label_fifo.Name = "label_fifo";
            this.label_fifo.Size = new System.Drawing.Size(55, 13);
            this.label_fifo.TabIndex = 14;
            this.label_fifo.Text = "TX Buffer:";
            // 
            // bt_blockinfo
            // 
            this.bt_blockinfo.Location = new System.Drawing.Point(1213, 591);
            this.bt_blockinfo.Name = "bt_blockinfo";
            this.bt_blockinfo.Size = new System.Drawing.Size(68, 23);
            this.bt_blockinfo.TabIndex = 13;
            this.bt_blockinfo.Text = "Block Info";
            this.bt_blockinfo.UseVisualStyleBackColor = true;
            this.bt_blockinfo.Click += new System.EventHandler(this.bt_blockinfo_Click);
            // 
            // progressBar_capfifo
            // 
            this.progressBar_capfifo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(128)))));
            this.progressBar_capfifo.Location = new System.Drawing.Point(658, 642);
            this.progressBar_capfifo.Name = "progressBar_capfifo";
            this.progressBar_capfifo.Size = new System.Drawing.Size(304, 18);
            this.progressBar_capfifo.Step = 1;
            this.progressBar_capfifo.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar_capfifo.TabIndex = 15;
            // 
            // label_capfifo
            // 
            this.label_capfifo.AutoSize = true;
            this.label_capfifo.Location = new System.Drawing.Point(567, 643);
            this.label_capfifo.Name = "label_capfifo";
            this.label_capfifo.Size = new System.Drawing.Size(56, 13);
            this.label_capfifo.TabIndex = 16;
            this.label_capfifo.Text = "RX Buffer:";
            // 
            // lb_rxsignal
            // 
            this.lb_rxsignal.AutoSize = true;
            this.lb_rxsignal.Location = new System.Drawing.Point(984, 643);
            this.lb_rxsignal.Name = "lb_rxsignal";
            this.lb_rxsignal.Size = new System.Drawing.Size(57, 13);
            this.lb_rxsignal.TabIndex = 18;
            this.lb_rxsignal.Text = "RX Signal:";
            // 
            // lb_rxsync
            // 
            this.lb_rxsync.AutoSize = true;
            this.lb_rxsync.Location = new System.Drawing.Point(1094, 643);
            this.lb_rxsync.Name = "lb_rxsync";
            this.lb_rxsync.Size = new System.Drawing.Size(52, 13);
            this.lb_rxsync.TabIndex = 20;
            this.lb_rxsync.Text = "RX Sync:";
            // 
            // pb_rxsync
            // 
            this.pb_rxsync.BackgroundImage = global::oscardata.Properties.Resources.redmarker;
            this.pb_rxsync.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_rxsync.Location = new System.Drawing.Point(1155, 636);
            this.pb_rxsync.Name = "pb_rxsync";
            this.pb_rxsync.Size = new System.Drawing.Size(24, 24);
            this.pb_rxsync.TabIndex = 19;
            this.pb_rxsync.TabStop = false;
            // 
            // pb_rxsignal
            // 
            this.pb_rxsignal.BackgroundImage = global::oscardata.Properties.Resources.redmarker;
            this.pb_rxsignal.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_rxsignal.Location = new System.Drawing.Point(1047, 636);
            this.pb_rxsignal.Name = "pb_rxsignal";
            this.pb_rxsignal.Size = new System.Drawing.Size(24, 24);
            this.pb_rxsignal.TabIndex = 17;
            this.pb_rxsignal.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1293, 691);
            this.Controls.Add(this.lb_rxsync);
            this.Controls.Add(this.pb_rxsync);
            this.Controls.Add(this.lb_rxsignal);
            this.Controls.Add(this.pb_rxsignal);
            this.Controls.Add(this.label_capfifo);
            this.Controls.Add(this.progressBar_capfifo);
            this.Controls.Add(this.bt_blockinfo);
            this.Controls.Add(this.label_fifo);
            this.Controls.Add(this.progressBar_fifo);
            this.Controls.Add(this.cb_speed);
            this.Controls.Add(this.label_speed);
            this.Controls.Add(this.panel_txspectrum);
            this.Controls.Add(this.panel_constel);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.tabControl1);
            this.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "QO-100 NB Transponder HS Multimedia Modem AMSAT-DL V0.45 by DJ0ABR";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.tabPage_ber.ResumeLayout(false);
            this.tabPage_image.ResumeLayout(false);
            this.tabPage_image.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_rximage)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox_tximage)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPage_file.ResumeLayout(false);
            this.tabPage_file.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabPage_audio.ResumeLayout(false);
            this.groupBox7.ResumeLayout(false);
            this.groupBox7.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            this.groupBox6.ResumeLayout(false);
            this.groupBox6.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pb_voice)).EndInit();
            this.groupBox5.ResumeLayout(false);
            this.groupBox5.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pb_voiceCAPstatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_voicePBstatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_mic)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_loadspeaker)).EndInit();
            this.tabPage_setup.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pb_audioCAPstatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_audioPBstatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_CAPvol)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_PBvol)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.tabPage_about.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pb_rxsync)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_rxsignal)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Timer timer_udpTX;
        private System.Windows.Forms.Timer timer_udprx;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel;
        private System.Windows.Forms.Panel panel_constel;
        private System.Windows.Forms.Timer timer_qpsk;
        private System.Windows.Forms.Panel panel_txspectrum;
        private System.Windows.Forms.TabPage tabPage_ber;
        private System.Windows.Forms.Button button_stopBERtest;
        private System.Windows.Forms.Button button_startBERtest;
        private System.Windows.Forms.RichTextBox rtb;
        private System.Windows.Forms.TabPage tabPage_image;
        private System.Windows.Forms.ComboBox comboBox_quality;
        private System.Windows.Forms.Button button_loadimage;
        private System.Windows.Forms.Button button_cancelimg;
        private System.Windows.Forms.Button button_sendimage;
        private System.Windows.Forms.CheckBox checkBox_big;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label_rximage;
        private System.Windows.Forms.Label label_tximage;
        private System.Windows.Forms.PictureBox pictureBox_rximage;
        private System.Windows.Forms.PictureBox pictureBox_tximage;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.ToolStripStatusLabel ts_ip;
        private System.Windows.Forms.Panel groupBox1;
        private System.Windows.Forms.TabPage tabPage_file;
        private System.Windows.Forms.RichTextBox rtb_TXfile;
        private System.Windows.Forms.Button bt_file_send;
        private System.Windows.Forms.Button bt_sendBinaryFile;
        private System.Windows.Forms.Button bt_file_html;
        private System.Windows.Forms.Button bt_file_ascii;
        private System.Windows.Forms.RichTextBox rtb_RXfile;
        private System.Windows.Forms.Label label_rxfile;
        private System.Windows.Forms.Label label_txfile;
        private System.Windows.Forms.ToolStripStatusLabel RXstatus;
        private System.Windows.Forms.ComboBox cb_speed;
        private System.Windows.Forms.Label label_speed;
        private System.Windows.Forms.Timer timer_searchmodem;
        private System.Windows.Forms.Button bt_rximages;
        private System.Windows.Forms.Button bt_openrxfile;
        private System.Windows.Forms.CheckBox cb_loop;
        private System.Windows.Forms.Label label_nextimage;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TabPage tabPage_setup;
        private System.Windows.Forms.TextBox tb_callsign;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox cb_stampcall;
        private System.Windows.Forms.TextBox tb_shutdown;
        private System.Windows.Forms.Button bt_shutdown;
        private System.Windows.Forms.Button bt_resetmodem;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox cb_audioCAP;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cb_audioPB;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TrackBar tb_PBvol;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TrackBar tb_CAPvol;
        private System.Windows.Forms.ProgressBar progressBar_fifo;
        private System.Windows.Forms.Label label_fifo;
        private System.Windows.Forms.CheckBox cb_autostart;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.ComboBox cb_announcement;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.CheckBox cb_stampinfo;
        private System.Windows.Forms.TextBox tb_info;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TabPage tabPage_audio;
        private System.Windows.Forms.GroupBox groupBox5;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TrackBar tb_mic;
        private System.Windows.Forms.TrackBar tb_loadspeaker;
        private System.Windows.Forms.ComboBox cb_loudspeaker;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.ComboBox cb_mic;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Button bt_blockinfo;
        private System.Windows.Forms.CheckBox cb_switchtoLS;
        private System.Windows.Forms.GroupBox groupBox6;
        private System.Windows.Forms.CheckBox cb_voiceloop;
        private System.Windows.Forms.CheckBox cb_codecloop;
        private System.Windows.Forms.CheckBox cb_digitalVoice;
        private System.Windows.Forms.GroupBox groupBox7;
        private System.Windows.Forms.RadioButton rb_opus;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.TabPage tabPage_about;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private System.Windows.Forms.PictureBox pb_voiceCAPstatus;
        private System.Windows.Forms.PictureBox pb_voicePBstatus;
        private System.Windows.Forms.PictureBox pb_audioCAPstatus;
        private System.Windows.Forms.PictureBox pb_audioPBstatus;
        private System.Windows.Forms.RadioButton rb_codec2;
        private System.Windows.Forms.CheckBox cb_digitalVoiceRXonly;
        private System.Windows.Forms.PictureBox pb_voice;
        private System.Windows.Forms.ProgressBar progressBar_capfifo;
        private System.Windows.Forms.Label label_capfifo;
        private System.Windows.Forms.PictureBox pb_rxsignal;
        private System.Windows.Forms.Label lb_rxsignal;
        private System.Windows.Forms.Label lb_rxsync;
        private System.Windows.Forms.PictureBox pb_rxsync;
        private System.Windows.Forms.ComboBox cb_language;
    }
}


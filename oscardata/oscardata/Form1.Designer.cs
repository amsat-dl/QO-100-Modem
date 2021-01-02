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
            this.toolStrip_Type = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripStatusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.ts_ip = new System.Windows.Forms.ToolStripStatusLabel();
            this.RXstatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.panel_constel = new System.Windows.Forms.Panel();
            this.timer_qpsk = new System.Windows.Forms.Timer(this.components);
            this.panel_txspectrum = new System.Windows.Forms.Panel();
            this.tabPage_ber = new System.Windows.Forms.TabPage();
            this.bt_allf = new System.Windows.Forms.Button();
            this.lb_tuningqrgs = new System.Windows.Forms.Label();
            this.rtb = new System.Windows.Forms.RichTextBox();
            this.button_stopBERtest = new System.Windows.Forms.Button();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.button_startBERtest = new System.Windows.Forms.Button();
            this.tabPage_image = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.Panel();
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
            this.tabPage_rtty = new System.Windows.Forms.TabPage();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.tb_rtty_TX = new System.Windows.Forms.TextBox();
            this.tb_rtty_RX = new System.Windows.Forms.TextBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.rb_rtty_real = new System.Windows.Forms.RadioButton();
            this.bt_rtty_text6 = new System.Windows.Forms.Button();
            this.bt_rtty_text5 = new System.Windows.Forms.Button();
            this.bt_rtty_text4 = new System.Windows.Forms.Button();
            this.bt_rtty_text3 = new System.Windows.Forms.Button();
            this.bt_rtty_text2 = new System.Windows.Forms.Button();
            this.bt_rtty_text1 = new System.Windows.Forms.Button();
            this.button5 = new System.Windows.Forms.Button();
            this.bt_rtty_RY = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.button4 = new System.Windows.Forms.Button();
            this.bt_rtty_cq = new System.Windows.Forms.Button();
            this.bt_rxfont = new System.Windows.Forms.Button();
            this.bt_rtty_answerCQ = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.bt_rtty_start = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.bt_rtty_end = new System.Windows.Forms.Button();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.bt_rtty_endqso = new System.Windows.Forms.Button();
            this.rb_rtty_edit = new System.Windows.Forms.RadioButton();
            this.bt_rtty_myinfo = new System.Windows.Forms.Button();
            this.rb_rtty_normal = new System.Windows.Forms.RadioButton();
            this.bt_rtty_station = new System.Windows.Forms.Button();
            this.bt_rtty_default = new System.Windows.Forms.Button();
            this.bt_rtty_tx = new System.Windows.Forms.Button();
            this.label_urname = new System.Windows.Forms.Label();
            this.tb_rtty_deftext = new System.Windows.Forms.TextBox();
            this.tb_urname = new System.Windows.Forms.TextBox();
            this.tb_urcall = new System.Windows.Forms.TextBox();
            this.label_urcall = new System.Windows.Forms.Label();
            this.tabPage_setup = new System.Windows.Forms.TabPage();
            this.label_cfgpath = new System.Windows.Forms.Label();
            this.label_cfgpath_tit = new System.Windows.Forms.Label();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label12 = new System.Windows.Forms.Label();
            this.cb_safemode = new System.Windows.Forms.ComboBox();
            this.cb_language = new System.Windows.Forms.ComboBox();
            this.cb_autostart = new System.Windows.Forms.CheckBox();
            this.bt_shutdown = new System.Windows.Forms.Button();
            this.tb_shutdown = new System.Windows.Forms.TextBox();
            this.bt_resetmodem = new System.Windows.Forms.Button();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.pictureBox4 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.bt_tune_plus = new System.Windows.Forms.Button();
            this.bt_tune_minus = new System.Windows.Forms.Button();
            this.cb_marker = new System.Windows.Forms.CheckBox();
            this.pb_audioCAPstatus = new System.Windows.Forms.PictureBox();
            this.pb_audioPBstatus = new System.Windows.Forms.PictureBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.tb_CAPvol = new System.Windows.Forms.TrackBar();
            this.tb_PBvol = new System.Windows.Forms.TrackBar();
            this.cb_audioPB = new System.Windows.Forms.ComboBox();
            this.cb_audioCAP = new System.Windows.Forms.ComboBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.tb_myqthloc = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.tb_myqth = new System.Windows.Forms.TextBox();
            this.lb_qth = new System.Windows.Forms.Label();
            this.tb_myname = new System.Windows.Forms.TextBox();
            this.lb_myname = new System.Windows.Forms.Label();
            this.lb_rec = new System.Windows.Forms.Label();
            this.tb_recintro = new System.Windows.Forms.TextBox();
            this.bt_astop = new System.Windows.Forms.Button();
            this.bt_aplay = new System.Windows.Forms.Button();
            this.bt_arecord = new System.Windows.Forms.Button();
            this.cb_sendIntro = new System.Windows.Forms.CheckBox();
            this.cb_stampinfo = new System.Windows.Forms.CheckBox();
            this.tb_info = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
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
            this.label_fifo = new System.Windows.Forms.Label();
            this.label_capfifo = new System.Windows.Forms.Label();
            this.lb_rxsignal = new System.Windows.Forms.Label();
            this.lb_rxsync = new System.Windows.Forms.Label();
            this.pn1 = new System.Windows.Forms.Panel();
            this.pb_rxsignal = new System.Windows.Forms.PictureBox();
            this.pb_rxsync = new System.Windows.Forms.PictureBox();
            this.cb_rx_autosync = new System.Windows.Forms.CheckBox();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.progressBar_fifo = new oscardata.KmProgressBar();
            this.progressBar_capfifo = new oscardata.KmProgressBar();
            this.vu_cap = new oscardata.KmProgressBar();
            this.vu_pb = new oscardata.KmProgressBar();
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
            this.tabPage_rtty.SuspendLayout();
            this.panel1.SuspendLayout();
            this.tabPage_setup.SuspendLayout();
            this.groupBox4.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_audioCAPstatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_audioPBstatus)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_CAPvol)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_PBvol)).BeginInit();
            this.groupBox2.SuspendLayout();
            this.tabPage_about.SuspendLayout();
            this.pn1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pb_rxsignal)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_rxsync)).BeginInit();
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
            this.toolStrip_Type,
            this.toolStripStatusLabel,
            this.ts_ip,
            this.RXstatus});
            this.statusStrip1.Location = new System.Drawing.Point(0, 671);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1296, 22);
            this.statusStrip1.TabIndex = 4;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStrip_Type
            // 
            this.toolStrip_Type.Font = new System.Drawing.Font("Verdana", 9F, System.Drawing.FontStyle.Bold);
            this.toolStrip_Type.ForeColor = System.Drawing.Color.Red;
            this.toolStrip_Type.Name = "toolStrip_Type";
            this.toolStrip_Type.Size = new System.Drawing.Size(0, 17);
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
            this.panel_constel.BackColor = System.Drawing.Color.AliceBlue;
            this.panel_constel.Location = new System.Drawing.Point(11, 590);
            this.panel_constel.Name = "panel_constel";
            this.panel_constel.Size = new System.Drawing.Size(75, 75);
            this.panel_constel.TabIndex = 5;
            this.panel_constel.Paint += new System.Windows.Forms.PaintEventHandler(this.panel_constel_Paint);
            // 
            // timer_qpsk
            // 
            this.timer_qpsk.Enabled = true;
            this.timer_qpsk.Interval = 200;
            this.timer_qpsk.Tick += new System.EventHandler(this.timer_qpsk_Tick);
            // 
            // panel_txspectrum
            // 
            this.panel_txspectrum.BackColor = System.Drawing.SystemColors.ControlLight;
            this.panel_txspectrum.Location = new System.Drawing.Point(92, 590);
            this.panel_txspectrum.Name = "panel_txspectrum";
            this.panel_txspectrum.Size = new System.Drawing.Size(442, 76);
            this.panel_txspectrum.TabIndex = 6;
            this.panel_txspectrum.Paint += new System.Windows.Forms.PaintEventHandler(this.panel_txspectrum_Paint);
            this.panel_txspectrum.DoubleClick += new System.EventHandler(this.panel_txspectrum_DoubleClick);
            // 
            // tabPage_ber
            // 
            this.tabPage_ber.BackColor = System.Drawing.Color.Transparent;
            this.tabPage_ber.Controls.Add(this.bt_allf);
            this.tabPage_ber.Controls.Add(this.lb_tuningqrgs);
            this.tabPage_ber.Controls.Add(this.rtb);
            this.tabPage_ber.Controls.Add(this.button_stopBERtest);
            this.tabPage_ber.Controls.Add(this.button_startBERtest);
            this.tabPage_ber.ImageIndex = 4;
            this.tabPage_ber.Location = new System.Drawing.Point(4, 23);
            this.tabPage_ber.Name = "tabPage_ber";
            this.tabPage_ber.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_ber.Size = new System.Drawing.Size(1280, 552);
            this.tabPage_ber.TabIndex = 0;
            this.tabPage_ber.Text = "BER Test";
            // 
            // bt_allf
            // 
            this.bt_allf.Location = new System.Drawing.Point(561, 18);
            this.bt_allf.Name = "bt_allf";
            this.bt_allf.Size = new System.Drawing.Size(71, 23);
            this.bt_allf.TabIndex = 25;
            this.bt_allf.Text = "1500 Hz";
            this.bt_allf.UseVisualStyleBackColor = true;
            this.bt_allf.Click += new System.EventHandler(this.bt_allf_Click);
            // 
            // lb_tuningqrgs
            // 
            this.lb_tuningqrgs.AutoSize = true;
            this.lb_tuningqrgs.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lb_tuningqrgs.Location = new System.Drawing.Point(436, 22);
            this.lb_tuningqrgs.Name = "lb_tuningqrgs";
            this.lb_tuningqrgs.Size = new System.Drawing.Size(84, 13);
            this.lb_tuningqrgs.TabIndex = 21;
            this.lb_tuningqrgs.Text = "Frequency Test:";
            // 
            // rtb
            // 
            this.rtb.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rtb.Location = new System.Drawing.Point(6, 51);
            this.rtb.Name = "rtb";
            this.rtb.Size = new System.Drawing.Size(1266, 494);
            this.rtb.TabIndex = 0;
            this.rtb.Text = "";
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
            this.imageList1.Images.SetKeyName(13, "rtty.png");
            this.imageList1.Images.SetKeyName(14, "home-icon.png");
            this.imageList1.Images.SetKeyName(15, "ryry-icon.png");
            this.imageList1.Images.SetKeyName(16, "voice-icon.png");
            this.imageList1.Images.SetKeyName(17, "user-icon.png");
            this.imageList1.Images.SetKeyName(18, "answer.png");
            this.imageList1.Images.SetKeyName(19, "stop-icon.png");
            this.imageList1.Images.SetKeyName(20, "start-icon.png");
            this.imageList1.Images.SetKeyName(21, "endqso-icon.png");
            this.imageList1.Images.SetKeyName(22, "text-icon.png");
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
            // tabPage_image
            // 
            this.tabPage_image.BackColor = System.Drawing.Color.Transparent;
            this.tabPage_image.Controls.Add(this.groupBox1);
            this.tabPage_image.Controls.Add(this.label_rximage);
            this.tabPage_image.Controls.Add(this.label_tximage);
            this.tabPage_image.Controls.Add(this.pictureBox_rximage);
            this.tabPage_image.Controls.Add(this.pictureBox_tximage);
            this.tabPage_image.ImageIndex = 3;
            this.tabPage_image.Location = new System.Drawing.Point(4, 23);
            this.tabPage_image.Name = "tabPage_image";
            this.tabPage_image.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage_image.Size = new System.Drawing.Size(1280, 552);
            this.tabPage_image.TabIndex = 1;
            this.tabPage_image.Text = "Image";
            // 
            // groupBox1
            // 
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
            this.pictureBox_rximage.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox_rximage.BackgroundImage")));
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
            this.pictureBox_tximage.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox_tximage.BackgroundImage")));
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
            this.tabControl1.Controls.Add(this.tabPage_rtty);
            this.tabControl1.Controls.Add(this.tabPage_setup);
            this.tabControl1.Controls.Add(this.tabPage_about);
            this.tabControl1.ImageList = this.imageList1;
            this.tabControl1.Location = new System.Drawing.Point(5, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(1288, 579);
            this.tabControl1.TabIndex = 3;
            // 
            // tabPage_file
            // 
            this.tabPage_file.BackColor = System.Drawing.Color.Transparent;
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
            this.tabPage_file.Size = new System.Drawing.Size(1280, 552);
            this.tabPage_file.TabIndex = 2;
            this.tabPage_file.Text = "File";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox1.BackgroundImage")));
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
            this.tabPage_audio.BackColor = System.Drawing.Color.Transparent;
            this.tabPage_audio.Controls.Add(this.groupBox7);
            this.tabPage_audio.Controls.Add(this.groupBox6);
            this.tabPage_audio.Controls.Add(this.groupBox5);
            this.tabPage_audio.ImageIndex = 11;
            this.tabPage_audio.Location = new System.Drawing.Point(4, 23);
            this.tabPage_audio.Name = "tabPage_audio";
            this.tabPage_audio.Size = new System.Drawing.Size(1280, 552);
            this.tabPage_audio.TabIndex = 5;
            this.tabPage_audio.Text = "Voice Audio";
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
            this.rb_codec2.Checked = true;
            this.rb_codec2.Location = new System.Drawing.Point(92, 50);
            this.rb_codec2.Name = "rb_codec2";
            this.rb_codec2.Size = new System.Drawing.Size(427, 17);
            this.rb_codec2.TabIndex = 23;
            this.rb_codec2.TabStop = true;
            this.rb_codec2.Text = "CODEC-2 parametric audio codec. For BPSK/QPSK. Audio rate: 700/1600/3200 bps";
            this.rb_codec2.UseVisualStyleBackColor = true;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox2.BackgroundImage")));
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
            this.rb_opus.Location = new System.Drawing.Point(92, 27);
            this.rb_opus.Name = "rb_opus";
            this.rb_opus.Size = new System.Drawing.Size(345, 17);
            this.rb_opus.TabIndex = 0;
            this.rb_opus.Text = "OPUS rate adaptive codec. For 8APSK. Audio rate: 4600..6000 bps";
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
            this.pb_voiceCAPstatus.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pb_voiceCAPstatus.BackgroundImage")));
            this.pb_voiceCAPstatus.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_voiceCAPstatus.Location = new System.Drawing.Point(387, 58);
            this.pb_voiceCAPstatus.Name = "pb_voiceCAPstatus";
            this.pb_voiceCAPstatus.Size = new System.Drawing.Size(21, 21);
            this.pb_voiceCAPstatus.TabIndex = 19;
            this.pb_voiceCAPstatus.TabStop = false;
            // 
            // pb_voicePBstatus
            // 
            this.pb_voicePBstatus.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pb_voicePBstatus.BackgroundImage")));
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
            // tabPage_rtty
            // 
            this.tabPage_rtty.Controls.Add(this.label4);
            this.tabPage_rtty.Controls.Add(this.label3);
            this.tabPage_rtty.Controls.Add(this.tb_rtty_TX);
            this.tabPage_rtty.Controls.Add(this.tb_rtty_RX);
            this.tabPage_rtty.Controls.Add(this.panel1);
            this.tabPage_rtty.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tabPage_rtty.ImageIndex = 13;
            this.tabPage_rtty.Location = new System.Drawing.Point(4, 23);
            this.tabPage_rtty.Name = "tabPage_rtty";
            this.tabPage_rtty.Size = new System.Drawing.Size(1280, 552);
            this.tabPage_rtty.TabIndex = 7;
            this.tabPage_rtty.Text = "RTTY";
            this.tabPage_rtty.UseVisualStyleBackColor = true;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(13, 393);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(29, 20);
            this.label4.TabIndex = 32;
            this.label4.Text = "TX";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(13, 13);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(32, 20);
            this.label3.TabIndex = 26;
            this.label3.Text = "RX";
            // 
            // tb_rtty_TX
            // 
            this.tb_rtty_TX.BackColor = System.Drawing.Color.AliceBlue;
            this.tb_rtty_TX.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb_rtty_TX.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.tb_rtty_TX.Font = new System.Drawing.Font("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_rtty_TX.Location = new System.Drawing.Point(9, 416);
            this.tb_rtty_TX.Multiline = true;
            this.tb_rtty_TX.Name = "tb_rtty_TX";
            this.tb_rtty_TX.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tb_rtty_TX.Size = new System.Drawing.Size(675, 103);
            this.tb_rtty_TX.TabIndex = 1;
            this.tb_rtty_TX.TextChanged += new System.EventHandler(this.tb_rtty_TX_TextChanged);
            this.tb_rtty_TX.KeyDown += new System.Windows.Forms.KeyEventHandler(this.tb_rtty_TX_KeyDown);
            // 
            // tb_rtty_RX
            // 
            this.tb_rtty_RX.BackColor = System.Drawing.Color.Cornsilk;
            this.tb_rtty_RX.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb_rtty_RX.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.tb_rtty_RX.Font = new System.Drawing.Font("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_rtty_RX.Location = new System.Drawing.Point(9, 35);
            this.tb_rtty_RX.Multiline = true;
            this.tb_rtty_RX.Name = "tb_rtty_RX";
            this.tb_rtty_RX.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.tb_rtty_RX.Size = new System.Drawing.Size(675, 348);
            this.tb_rtty_RX.TabIndex = 2;
            this.tb_rtty_RX.MouseDown += new System.Windows.Forms.MouseEventHandler(this.tb_rtty_RX_MouseDown);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Transparent;
            this.panel1.Controls.Add(this.textBox6);
            this.panel1.Controls.Add(this.cb_rx_autosync);
            this.panel1.Controls.Add(this.rb_rtty_real);
            this.panel1.Controls.Add(this.bt_rtty_text6);
            this.panel1.Controls.Add(this.bt_rtty_text5);
            this.panel1.Controls.Add(this.bt_rtty_text4);
            this.panel1.Controls.Add(this.bt_rtty_text3);
            this.panel1.Controls.Add(this.bt_rtty_text2);
            this.panel1.Controls.Add(this.bt_rtty_text1);
            this.panel1.Controls.Add(this.button5);
            this.panel1.Controls.Add(this.bt_rtty_RY);
            this.panel1.Controls.Add(this.textBox2);
            this.panel1.Controls.Add(this.button4);
            this.panel1.Controls.Add(this.bt_rtty_cq);
            this.panel1.Controls.Add(this.bt_rxfont);
            this.panel1.Controls.Add(this.bt_rtty_answerCQ);
            this.panel1.Controls.Add(this.button3);
            this.panel1.Controls.Add(this.bt_rtty_start);
            this.panel1.Controls.Add(this.button1);
            this.panel1.Controls.Add(this.bt_rtty_end);
            this.panel1.Controls.Add(this.textBox5);
            this.panel1.Controls.Add(this.bt_rtty_endqso);
            this.panel1.Controls.Add(this.rb_rtty_edit);
            this.panel1.Controls.Add(this.bt_rtty_myinfo);
            this.panel1.Controls.Add(this.rb_rtty_normal);
            this.panel1.Controls.Add(this.bt_rtty_station);
            this.panel1.Controls.Add(this.bt_rtty_default);
            this.panel1.Controls.Add(this.bt_rtty_tx);
            this.panel1.Controls.Add(this.label_urname);
            this.panel1.Controls.Add(this.tb_rtty_deftext);
            this.panel1.Controls.Add(this.tb_urname);
            this.panel1.Controls.Add(this.tb_urcall);
            this.panel1.Controls.Add(this.label_urcall);
            this.panel1.Location = new System.Drawing.Point(696, 8);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(564, 514);
            this.panel1.TabIndex = 31;
            // 
            // rb_rtty_real
            // 
            this.rb_rtty_real.AutoSize = true;
            this.rb_rtty_real.Location = new System.Drawing.Point(373, 203);
            this.rb_rtty_real.Name = "rb_rtty_real";
            this.rb_rtty_real.Size = new System.Drawing.Size(47, 17);
            this.rb_rtty_real.TabIndex = 33;
            this.rb_rtty_real.Text = "Real";
            this.rb_rtty_real.UseVisualStyleBackColor = true;
            this.rb_rtty_real.CheckedChanged += new System.EventHandler(this.rb_rtty_real_CheckedChanged);
            // 
            // bt_rtty_text6
            // 
            this.bt_rtty_text6.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_text6.ImageIndex = 22;
            this.bt_rtty_text6.ImageList = this.imageList1;
            this.bt_rtty_text6.Location = new System.Drawing.Point(432, 293);
            this.bt_rtty_text6.Name = "bt_rtty_text6";
            this.bt_rtty_text6.Size = new System.Drawing.Size(107, 25);
            this.bt_rtty_text6.TabIndex = 32;
            this.bt_rtty_text6.Text = "TEXT 6";
            this.bt_rtty_text6.UseVisualStyleBackColor = true;
            this.bt_rtty_text6.Click += new System.EventHandler(this.bt_rtty_text6_Click);
            // 
            // bt_rtty_text5
            // 
            this.bt_rtty_text5.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_text5.ImageIndex = 22;
            this.bt_rtty_text5.ImageList = this.imageList1;
            this.bt_rtty_text5.Location = new System.Drawing.Point(432, 264);
            this.bt_rtty_text5.Name = "bt_rtty_text5";
            this.bt_rtty_text5.Size = new System.Drawing.Size(107, 25);
            this.bt_rtty_text5.TabIndex = 31;
            this.bt_rtty_text5.Text = "TEXT 5";
            this.bt_rtty_text5.UseVisualStyleBackColor = true;
            this.bt_rtty_text5.Click += new System.EventHandler(this.bt_rtty_text5_Click);
            // 
            // bt_rtty_text4
            // 
            this.bt_rtty_text4.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_text4.ImageIndex = 22;
            this.bt_rtty_text4.ImageList = this.imageList1;
            this.bt_rtty_text4.Location = new System.Drawing.Point(432, 235);
            this.bt_rtty_text4.Name = "bt_rtty_text4";
            this.bt_rtty_text4.Size = new System.Drawing.Size(107, 25);
            this.bt_rtty_text4.TabIndex = 30;
            this.bt_rtty_text4.Text = "TEXT 4";
            this.bt_rtty_text4.UseVisualStyleBackColor = true;
            this.bt_rtty_text4.Click += new System.EventHandler(this.bt_rtty_text4_Click);
            // 
            // bt_rtty_text3
            // 
            this.bt_rtty_text3.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_text3.ImageIndex = 22;
            this.bt_rtty_text3.ImageList = this.imageList1;
            this.bt_rtty_text3.Location = new System.Drawing.Point(319, 293);
            this.bt_rtty_text3.Name = "bt_rtty_text3";
            this.bt_rtty_text3.Size = new System.Drawing.Size(107, 25);
            this.bt_rtty_text3.TabIndex = 29;
            this.bt_rtty_text3.Text = "TEXT 3";
            this.bt_rtty_text3.UseVisualStyleBackColor = true;
            this.bt_rtty_text3.Click += new System.EventHandler(this.bt_rtty_text3_Click);
            // 
            // bt_rtty_text2
            // 
            this.bt_rtty_text2.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_text2.ImageIndex = 22;
            this.bt_rtty_text2.ImageList = this.imageList1;
            this.bt_rtty_text2.Location = new System.Drawing.Point(319, 264);
            this.bt_rtty_text2.Name = "bt_rtty_text2";
            this.bt_rtty_text2.Size = new System.Drawing.Size(107, 25);
            this.bt_rtty_text2.TabIndex = 28;
            this.bt_rtty_text2.Text = "TEXT 2";
            this.bt_rtty_text2.UseVisualStyleBackColor = true;
            this.bt_rtty_text2.Click += new System.EventHandler(this.bt_rtty_text2_Click);
            // 
            // bt_rtty_text1
            // 
            this.bt_rtty_text1.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_text1.ImageIndex = 22;
            this.bt_rtty_text1.ImageList = this.imageList1;
            this.bt_rtty_text1.Location = new System.Drawing.Point(319, 235);
            this.bt_rtty_text1.Name = "bt_rtty_text1";
            this.bt_rtty_text1.Size = new System.Drawing.Size(107, 25);
            this.bt_rtty_text1.TabIndex = 27;
            this.bt_rtty_text1.Text = "TEXT 1";
            this.bt_rtty_text1.UseVisualStyleBackColor = true;
            this.bt_rtty_text1.Click += new System.EventHandler(this.bt_rtty_text1_Click);
            // 
            // button5
            // 
            this.button5.BackColor = System.Drawing.Color.Transparent;
            this.button5.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.button5.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button5.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.button5.ImageIndex = 1;
            this.button5.ImageList = this.imageList1;
            this.button5.Location = new System.Drawing.Point(39, 123);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(114, 38);
            this.button5.TabIndex = 26;
            this.button5.Text = "Stop TX";
            this.button5.UseVisualStyleBackColor = false;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // bt_rtty_RY
            // 
            this.bt_rtty_RY.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_RY.ImageIndex = 15;
            this.bt_rtty_RY.ImageList = this.imageList1;
            this.bt_rtty_RY.Location = new System.Drawing.Point(179, 293);
            this.bt_rtty_RY.Name = "bt_rtty_RY";
            this.bt_rtty_RY.Size = new System.Drawing.Size(134, 25);
            this.bt_rtty_RY.TabIndex = 25;
            this.bt_rtty_RY.Text = "RYRYRY...";
            this.bt_rtty_RY.UseVisualStyleBackColor = true;
            this.bt_rtty_RY.Click += new System.EventHandler(this.bt_rtty_RY_Click);
            // 
            // textBox2
            // 
            this.textBox2.BackColor = System.Drawing.SystemColors.Control;
            this.textBox2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox2.Enabled = false;
            this.textBox2.Font = new System.Drawing.Font("Courier New", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox2.Location = new System.Drawing.Point(305, 18);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.ReadOnly = true;
            this.textBox2.Size = new System.Drawing.Size(186, 143);
            this.textBox2.TabIndex = 12;
            this.textBox2.Text = "Special Markers:\r\n%m ... my call\r\n%c ... ur call\r\n%n ... ur name\r\n%r ... stop TX\r" +
    "\n";
            // 
            // button4
            // 
            this.button4.BackgroundImage = global::oscardata.Properties.Resources.foht;
            this.button4.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.button4.Location = new System.Drawing.Point(1, 429);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(24, 24);
            this.button4.TabIndex = 24;
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // bt_rtty_cq
            // 
            this.bt_rtty_cq.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_cq.ImageIndex = 16;
            this.bt_rtty_cq.ImageList = this.imageList1;
            this.bt_rtty_cq.Location = new System.Drawing.Point(39, 177);
            this.bt_rtty_cq.Name = "bt_rtty_cq";
            this.bt_rtty_cq.Size = new System.Drawing.Size(134, 25);
            this.bt_rtty_cq.TabIndex = 3;
            this.bt_rtty_cq.Text = "Call CQ";
            this.bt_rtty_cq.UseVisualStyleBackColor = true;
            this.bt_rtty_cq.Click += new System.EventHandler(this.bt_rtty_cq_Click);
            // 
            // bt_rxfont
            // 
            this.bt_rxfont.BackgroundImage = global::oscardata.Properties.Resources.foht;
            this.bt_rxfont.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.bt_rxfont.Location = new System.Drawing.Point(1, 59);
            this.bt_rxfont.Name = "bt_rxfont";
            this.bt_rxfont.Size = new System.Drawing.Size(24, 24);
            this.bt_rxfont.TabIndex = 23;
            this.bt_rxfont.UseVisualStyleBackColor = true;
            this.bt_rxfont.Click += new System.EventHandler(this.bt_rxfont_Click);
            // 
            // bt_rtty_answerCQ
            // 
            this.bt_rtty_answerCQ.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_answerCQ.ImageIndex = 18;
            this.bt_rtty_answerCQ.ImageList = this.imageList1;
            this.bt_rtty_answerCQ.Location = new System.Drawing.Point(39, 206);
            this.bt_rtty_answerCQ.Name = "bt_rtty_answerCQ";
            this.bt_rtty_answerCQ.Size = new System.Drawing.Size(134, 25);
            this.bt_rtty_answerCQ.TabIndex = 4;
            this.bt_rtty_answerCQ.Text = "Answer CQ Call";
            this.bt_rtty_answerCQ.UseVisualStyleBackColor = true;
            this.bt_rtty_answerCQ.Click += new System.EventHandler(this.bt_rtty_answerCQ_Click);
            // 
            // button3
            // 
            this.button3.BackgroundImage = global::oscardata.Properties.Resources.clearscreen;
            this.button3.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.button3.Location = new System.Drawing.Point(1, 399);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(24, 24);
            this.button3.TabIndex = 22;
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // bt_rtty_start
            // 
            this.bt_rtty_start.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_start.ImageIndex = 20;
            this.bt_rtty_start.ImageList = this.imageList1;
            this.bt_rtty_start.Location = new System.Drawing.Point(39, 235);
            this.bt_rtty_start.Name = "bt_rtty_start";
            this.bt_rtty_start.Size = new System.Drawing.Size(134, 25);
            this.bt_rtty_start.TabIndex = 5;
            this.bt_rtty_start.Text = "Start Transmission";
            this.bt_rtty_start.UseVisualStyleBackColor = true;
            this.bt_rtty_start.Click += new System.EventHandler(this.bt_rtty_start_Click);
            // 
            // button1
            // 
            this.button1.BackgroundImage = global::oscardata.Properties.Resources.clearscreen;
            this.button1.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.button1.Location = new System.Drawing.Point(1, 29);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(24, 24);
            this.button1.TabIndex = 21;
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click_1);
            // 
            // bt_rtty_end
            // 
            this.bt_rtty_end.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_end.ImageIndex = 19;
            this.bt_rtty_end.ImageList = this.imageList1;
            this.bt_rtty_end.Location = new System.Drawing.Point(39, 264);
            this.bt_rtty_end.Name = "bt_rtty_end";
            this.bt_rtty_end.Size = new System.Drawing.Size(134, 25);
            this.bt_rtty_end.TabIndex = 6;
            this.bt_rtty_end.Text = "End Transmission";
            this.bt_rtty_end.UseVisualStyleBackColor = true;
            this.bt_rtty_end.Click += new System.EventHandler(this.bt_rtty_end_Click);
            // 
            // textBox5
            // 
            this.textBox5.BackColor = System.Drawing.SystemColors.Control;
            this.textBox5.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.textBox5.Location = new System.Drawing.Point(171, 67);
            this.textBox5.Multiline = true;
            this.textBox5.Name = "textBox5";
            this.textBox5.ReadOnly = true;
            this.textBox5.Size = new System.Drawing.Size(117, 50);
            this.textBox5.TabIndex = 20;
            this.textBox5.Text = "Click Callsign and Names in RX-Windows";
            // 
            // bt_rtty_endqso
            // 
            this.bt_rtty_endqso.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_endqso.ImageIndex = 21;
            this.bt_rtty_endqso.ImageList = this.imageList1;
            this.bt_rtty_endqso.Location = new System.Drawing.Point(39, 293);
            this.bt_rtty_endqso.Name = "bt_rtty_endqso";
            this.bt_rtty_endqso.Size = new System.Drawing.Size(134, 25);
            this.bt_rtty_endqso.TabIndex = 7;
            this.bt_rtty_endqso.Text = "End QSO";
            this.bt_rtty_endqso.UseVisualStyleBackColor = true;
            this.bt_rtty_endqso.Click += new System.EventHandler(this.bt_rtty_endqso_Click);
            // 
            // rb_rtty_edit
            // 
            this.rb_rtty_edit.AutoSize = true;
            this.rb_rtty_edit.Location = new System.Drawing.Point(436, 203);
            this.rb_rtty_edit.Name = "rb_rtty_edit";
            this.rb_rtty_edit.Size = new System.Drawing.Size(43, 17);
            this.rb_rtty_edit.TabIndex = 19;
            this.rb_rtty_edit.Text = "Edit";
            this.rb_rtty_edit.UseVisualStyleBackColor = true;
            this.rb_rtty_edit.CheckedChanged += new System.EventHandler(this.rb_rtty_edit_CheckedChanged);
            // 
            // bt_rtty_myinfo
            // 
            this.bt_rtty_myinfo.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_myinfo.ImageIndex = 17;
            this.bt_rtty_myinfo.ImageList = this.imageList1;
            this.bt_rtty_myinfo.Location = new System.Drawing.Point(179, 235);
            this.bt_rtty_myinfo.Name = "bt_rtty_myinfo";
            this.bt_rtty_myinfo.Size = new System.Drawing.Size(134, 25);
            this.bt_rtty_myinfo.TabIndex = 8;
            this.bt_rtty_myinfo.Text = "My Info";
            this.bt_rtty_myinfo.UseVisualStyleBackColor = true;
            this.bt_rtty_myinfo.Click += new System.EventHandler(this.bt_rtty_myinfo_Click);
            // 
            // rb_rtty_normal
            // 
            this.rb_rtty_normal.AutoSize = true;
            this.rb_rtty_normal.Checked = true;
            this.rb_rtty_normal.Location = new System.Drawing.Point(320, 203);
            this.rb_rtty_normal.Name = "rb_rtty_normal";
            this.rb_rtty_normal.Size = new System.Drawing.Size(39, 17);
            this.rb_rtty_normal.TabIndex = 18;
            this.rb_rtty_normal.TabStop = true;
            this.rb_rtty_normal.Text = "TX";
            this.rb_rtty_normal.UseVisualStyleBackColor = true;
            this.rb_rtty_normal.CheckedChanged += new System.EventHandler(this.rb_rtty_normal_CheckedChanged);
            // 
            // bt_rtty_station
            // 
            this.bt_rtty_station.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_station.ImageIndex = 14;
            this.bt_rtty_station.ImageList = this.imageList1;
            this.bt_rtty_station.Location = new System.Drawing.Point(179, 264);
            this.bt_rtty_station.Name = "bt_rtty_station";
            this.bt_rtty_station.Size = new System.Drawing.Size(134, 25);
            this.bt_rtty_station.TabIndex = 9;
            this.bt_rtty_station.Text = "My Station";
            this.bt_rtty_station.UseVisualStyleBackColor = true;
            this.bt_rtty_station.Click += new System.EventHandler(this.bt_rtty_station_Click);
            // 
            // bt_rtty_default
            // 
            this.bt_rtty_default.Enabled = false;
            this.bt_rtty_default.Location = new System.Drawing.Point(305, 167);
            this.bt_rtty_default.Name = "bt_rtty_default";
            this.bt_rtty_default.Size = new System.Drawing.Size(186, 23);
            this.bt_rtty_default.TabIndex = 17;
            this.bt_rtty_default.Text = "set Default Text";
            this.bt_rtty_default.UseVisualStyleBackColor = true;
            this.bt_rtty_default.Click += new System.EventHandler(this.bt_rtty_default_Click);
            // 
            // bt_rtty_tx
            // 
            this.bt_rtty_tx.BackColor = System.Drawing.Color.LightGreen;
            this.bt_rtty_tx.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bt_rtty_tx.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.bt_rtty_tx.ImageIndex = 9;
            this.bt_rtty_tx.ImageList = this.imageList1;
            this.bt_rtty_tx.Location = new System.Drawing.Point(39, 79);
            this.bt_rtty_tx.Name = "bt_rtty_tx";
            this.bt_rtty_tx.Size = new System.Drawing.Size(114, 38);
            this.bt_rtty_tx.TabIndex = 10;
            this.bt_rtty_tx.Text = "TX on/off";
            this.bt_rtty_tx.UseVisualStyleBackColor = false;
            this.bt_rtty_tx.Click += new System.EventHandler(this.bt_rtty_tx_Click);
            // 
            // label_urname
            // 
            this.label_urname.AutoSize = true;
            this.label_urname.Location = new System.Drawing.Point(74, 46);
            this.label_urname.Name = "label_urname";
            this.label_urname.Size = new System.Drawing.Size(63, 13);
            this.label_urname.TabIndex = 16;
            this.label_urname.Text = "Your Name:";
            // 
            // tb_rtty_deftext
            // 
            this.tb_rtty_deftext.Font = new System.Drawing.Font("Courier New", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_rtty_deftext.Location = new System.Drawing.Point(39, 341);
            this.tb_rtty_deftext.Multiline = true;
            this.tb_rtty_deftext.Name = "tb_rtty_deftext";
            this.tb_rtty_deftext.Size = new System.Drawing.Size(522, 153);
            this.tb_rtty_deftext.TabIndex = 11;
            this.tb_rtty_deftext.TextChanged += new System.EventHandler(this.tb_rtty_deftext_TextChanged);
            // 
            // tb_urname
            // 
            this.tb_urname.Location = new System.Drawing.Point(171, 43);
            this.tb_urname.Name = "tb_urname";
            this.tb_urname.Size = new System.Drawing.Size(117, 20);
            this.tb_urname.TabIndex = 15;
            // 
            // tb_urcall
            // 
            this.tb_urcall.Location = new System.Drawing.Point(171, 17);
            this.tb_urcall.Name = "tb_urcall";
            this.tb_urcall.Size = new System.Drawing.Size(117, 20);
            this.tb_urcall.TabIndex = 13;
            // 
            // label_urcall
            // 
            this.label_urcall.AutoSize = true;
            this.label_urcall.Location = new System.Drawing.Point(74, 20);
            this.label_urcall.Name = "label_urcall";
            this.label_urcall.Size = new System.Drawing.Size(71, 13);
            this.label_urcall.TabIndex = 14;
            this.label_urcall.Text = "Your Callsign:";
            // 
            // tabPage_setup
            // 
            this.tabPage_setup.BackColor = System.Drawing.Color.Transparent;
            this.tabPage_setup.Controls.Add(this.label_cfgpath);
            this.tabPage_setup.Controls.Add(this.label_cfgpath_tit);
            this.tabPage_setup.Controls.Add(this.groupBox4);
            this.tabPage_setup.Controls.Add(this.groupBox3);
            this.tabPage_setup.Controls.Add(this.groupBox2);
            this.tabPage_setup.ImageIndex = 6;
            this.tabPage_setup.Location = new System.Drawing.Point(4, 23);
            this.tabPage_setup.Name = "tabPage_setup";
            this.tabPage_setup.Size = new System.Drawing.Size(1280, 552);
            this.tabPage_setup.TabIndex = 4;
            this.tabPage_setup.Text = "Setup";
            // 
            // label_cfgpath
            // 
            this.label_cfgpath.AutoSize = true;
            this.label_cfgpath.Location = new System.Drawing.Point(162, 441);
            this.label_cfgpath.Name = "label_cfgpath";
            this.label_cfgpath.Size = new System.Drawing.Size(16, 13);
            this.label_cfgpath.TabIndex = 17;
            this.label_cfgpath.Text = "...";
            // 
            // label_cfgpath_tit
            // 
            this.label_cfgpath_tit.AutoSize = true;
            this.label_cfgpath_tit.Location = new System.Drawing.Point(17, 441);
            this.label_cfgpath_tit.Name = "label_cfgpath_tit";
            this.label_cfgpath_tit.Size = new System.Drawing.Size(115, 13);
            this.label_cfgpath_tit.TabIndex = 16;
            this.label_cfgpath_tit.Text = "Configuration stored in:";
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.label13);
            this.groupBox4.Controls.Add(this.label12);
            this.groupBox4.Controls.Add(this.cb_safemode);
            this.groupBox4.Controls.Add(this.cb_language);
            this.groupBox4.Controls.Add(this.cb_autostart);
            this.groupBox4.Controls.Add(this.bt_shutdown);
            this.groupBox4.Controls.Add(this.tb_shutdown);
            this.groupBox4.Controls.Add(this.bt_resetmodem);
            this.groupBox4.Controls.Add(this.textBox3);
            this.groupBox4.Location = new System.Drawing.Point(12, 313);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(743, 121);
            this.groupBox4.TabIndex = 15;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Maintenance";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(240, 78);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(70, 13);
            this.label13.TabIndex = 27;
            this.label13.Text = "data security:";
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Location = new System.Drawing.Point(221, 23);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(103, 13);
            this.label12.TabIndex = 26;
            this.label12.Text = "Language/Sprache:";
            // 
            // cb_safemode
            // 
            this.cb_safemode.FormattingEnabled = true;
            this.cb_safemode.Items.AddRange(new object[] {
            "off (fast)",
            "medium",
            "high (slow)"});
            this.cb_safemode.Location = new System.Drawing.Point(326, 75);
            this.cb_safemode.Name = "cb_safemode";
            this.cb_safemode.Size = new System.Drawing.Size(110, 21);
            this.cb_safemode.TabIndex = 25;
            this.cb_safemode.Text = "off";
            // 
            // cb_language
            // 
            this.cb_language.FormattingEnabled = true;
            this.cb_language.Items.AddRange(new object[] {
            "English",
            "German/Deutsch"});
            this.cb_language.Location = new System.Drawing.Point(326, 19);
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
            this.tb_shutdown.BackColor = System.Drawing.SystemColors.Control;
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
            this.bt_resetmodem.Location = new System.Drawing.Point(529, 92);
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
            this.textBox3.BackColor = System.Drawing.SystemColors.Control;
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
            this.groupBox3.Controls.Add(this.pictureBox4);
            this.groupBox3.Controls.Add(this.pictureBox3);
            this.groupBox3.Controls.Add(this.bt_tune_plus);
            this.groupBox3.Controls.Add(this.bt_tune_minus);
            this.groupBox3.Controls.Add(this.cb_marker);
            this.groupBox3.Controls.Add(this.vu_cap);
            this.groupBox3.Controls.Add(this.vu_pb);
            this.groupBox3.Controls.Add(this.pb_audioCAPstatus);
            this.groupBox3.Controls.Add(this.pb_audioPBstatus);
            this.groupBox3.Controls.Add(this.label6);
            this.groupBox3.Controls.Add(this.label5);
            this.groupBox3.Controls.Add(this.tb_CAPvol);
            this.groupBox3.Controls.Add(this.tb_PBvol);
            this.groupBox3.Controls.Add(this.cb_audioPB);
            this.groupBox3.Controls.Add(this.cb_audioCAP);
            this.groupBox3.Location = new System.Drawing.Point(12, 144);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(743, 163);
            this.groupBox3.TabIndex = 14;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Transceiver Audio";
            // 
            // pictureBox4
            // 
            this.pictureBox4.BackgroundImage = global::oscardata.Properties.Resources.captureicon;
            this.pictureBox4.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox4.Location = new System.Drawing.Point(0, 61);
            this.pictureBox4.Name = "pictureBox4";
            this.pictureBox4.Size = new System.Drawing.Size(132, 87);
            this.pictureBox4.TabIndex = 16;
            this.pictureBox4.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pictureBox3.BackgroundImage")));
            this.pictureBox3.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pictureBox3.Location = new System.Drawing.Point(5, -7);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(127, 85);
            this.pictureBox3.TabIndex = 16;
            this.pictureBox3.TabStop = false;
            // 
            // bt_tune_plus
            // 
            this.bt_tune_plus.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bt_tune_plus.Location = new System.Drawing.Point(602, 79);
            this.bt_tune_plus.Name = "bt_tune_plus";
            this.bt_tune_plus.Size = new System.Drawing.Size(24, 23);
            this.bt_tune_plus.TabIndex = 22;
            this.bt_tune_plus.Text = "+";
            this.bt_tune_plus.UseVisualStyleBackColor = true;
            this.bt_tune_plus.Visible = false;
            this.bt_tune_plus.Click += new System.EventHandler(this.bt_tune_plus_Click);
            // 
            // bt_tune_minus
            // 
            this.bt_tune_minus.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.bt_tune_minus.Location = new System.Drawing.Point(602, 55);
            this.bt_tune_minus.Name = "bt_tune_minus";
            this.bt_tune_minus.Size = new System.Drawing.Size(24, 23);
            this.bt_tune_minus.TabIndex = 21;
            this.bt_tune_minus.Text = "-";
            this.bt_tune_minus.UseVisualStyleBackColor = true;
            this.bt_tune_minus.Visible = false;
            this.bt_tune_minus.Click += new System.EventHandler(this.bt_tune_minus_Click);
            // 
            // cb_marker
            // 
            this.cb_marker.AutoSize = true;
            this.cb_marker.Checked = true;
            this.cb_marker.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cb_marker.Location = new System.Drawing.Point(605, 23);
            this.cb_marker.Name = "cb_marker";
            this.cb_marker.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.cb_marker.Size = new System.Drawing.Size(129, 17);
            this.cb_marker.TabIndex = 27;
            this.cb_marker.Text = "100Hz Tuning Marker";
            this.cb_marker.UseVisualStyleBackColor = true;
            this.cb_marker.Visible = false;
            this.cb_marker.CheckedChanged += new System.EventHandler(this.cb_marker_CheckedChanged);
            // 
            // pb_audioCAPstatus
            // 
            this.pb_audioCAPstatus.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pb_audioCAPstatus.BackgroundImage")));
            this.pb_audioCAPstatus.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_audioCAPstatus.Location = new System.Drawing.Point(375, 87);
            this.pb_audioCAPstatus.Name = "pb_audioCAPstatus";
            this.pb_audioCAPstatus.Size = new System.Drawing.Size(21, 21);
            this.pb_audioCAPstatus.TabIndex = 18;
            this.pb_audioCAPstatus.TabStop = false;
            // 
            // pb_audioPBstatus
            // 
            this.pb_audioPBstatus.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pb_audioPBstatus.BackgroundImage")));
            this.pb_audioPBstatus.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_audioPBstatus.Location = new System.Drawing.Point(375, 21);
            this.pb_audioPBstatus.Name = "pb_audioPBstatus";
            this.pb_audioPBstatus.Size = new System.Drawing.Size(21, 21);
            this.pb_audioPBstatus.TabIndex = 17;
            this.pb_audioPBstatus.TabStop = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(428, 89);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(45, 13);
            this.label6.TabIndex = 16;
            this.label6.Text = "Volume:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(428, 25);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(45, 13);
            this.label5.TabIndex = 15;
            this.label5.Text = "Volume:";
            // 
            // tb_CAPvol
            // 
            this.tb_CAPvol.Cursor = System.Windows.Forms.Cursors.SizeAll;
            this.tb_CAPvol.Location = new System.Drawing.Point(479, 85);
            this.tb_CAPvol.Maximum = 100;
            this.tb_CAPvol.MaximumSize = new System.Drawing.Size(0, 24);
            this.tb_CAPvol.Minimum = 1;
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
            this.tb_PBvol.Location = new System.Drawing.Point(479, 21);
            this.tb_PBvol.Maximum = 100;
            this.tb_PBvol.MaximumSize = new System.Drawing.Size(0, 24);
            this.tb_PBvol.Minimum = 1;
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
            this.cb_audioPB.Location = new System.Drawing.Point(138, 21);
            this.cb_audioPB.Name = "cb_audioPB";
            this.cb_audioPB.Size = new System.Drawing.Size(230, 21);
            this.cb_audioPB.TabIndex = 7;
            this.cb_audioPB.Text = "Default";
            // 
            // cb_audioCAP
            // 
            this.cb_audioCAP.FormattingEnabled = true;
            this.cb_audioCAP.Location = new System.Drawing.Point(138, 87);
            this.cb_audioCAP.Name = "cb_audioCAP";
            this.cb_audioCAP.Size = new System.Drawing.Size(230, 21);
            this.cb_audioCAP.TabIndex = 9;
            this.cb_audioCAP.Text = "Default";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.tb_myqthloc);
            this.groupBox2.Controls.Add(this.label14);
            this.groupBox2.Controls.Add(this.tb_myqth);
            this.groupBox2.Controls.Add(this.lb_qth);
            this.groupBox2.Controls.Add(this.tb_myname);
            this.groupBox2.Controls.Add(this.lb_myname);
            this.groupBox2.Controls.Add(this.lb_rec);
            this.groupBox2.Controls.Add(this.tb_recintro);
            this.groupBox2.Controls.Add(this.bt_astop);
            this.groupBox2.Controls.Add(this.bt_aplay);
            this.groupBox2.Controls.Add(this.bt_arecord);
            this.groupBox2.Controls.Add(this.cb_sendIntro);
            this.groupBox2.Controls.Add(this.cb_stampinfo);
            this.groupBox2.Controls.Add(this.tb_info);
            this.groupBox2.Controls.Add(this.label7);
            this.groupBox2.Controls.Add(this.cb_announcement);
            this.groupBox2.Controls.Add(this.textBox4);
            this.groupBox2.Controls.Add(this.textBox1);
            this.groupBox2.Controls.Add(this.tb_callsign);
            this.groupBox2.Controls.Add(this.label1);
            this.groupBox2.Controls.Add(this.cb_stampcall);
            this.groupBox2.Location = new System.Drawing.Point(12, 13);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(743, 127);
            this.groupBox2.TabIndex = 13;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Personal Settings";
            // 
            // tb_myqthloc
            // 
            this.tb_myqthloc.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.tb_myqthloc.Location = new System.Drawing.Point(71, 99);
            this.tb_myqthloc.Name = "tb_myqthloc";
            this.tb_myqthloc.Size = new System.Drawing.Size(104, 20);
            this.tb_myqthloc.TabIndex = 35;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Location = new System.Drawing.Point(14, 102);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(54, 13);
            this.label14.TabIndex = 34;
            this.label14.Text = "QTHLOC:";
            // 
            // tb_myqth
            // 
            this.tb_myqth.Location = new System.Drawing.Point(71, 72);
            this.tb_myqth.Name = "tb_myqth";
            this.tb_myqth.Size = new System.Drawing.Size(104, 20);
            this.tb_myqth.TabIndex = 33;
            // 
            // lb_qth
            // 
            this.lb_qth.AutoSize = true;
            this.lb_qth.Location = new System.Drawing.Point(14, 75);
            this.lb_qth.Name = "lb_qth";
            this.lb_qth.Size = new System.Drawing.Size(33, 13);
            this.lb_qth.TabIndex = 32;
            this.lb_qth.Text = "QTH:";
            // 
            // tb_myname
            // 
            this.tb_myname.Location = new System.Drawing.Point(71, 46);
            this.tb_myname.Name = "tb_myname";
            this.tb_myname.Size = new System.Drawing.Size(104, 20);
            this.tb_myname.TabIndex = 31;
            // 
            // lb_myname
            // 
            this.lb_myname.AutoSize = true;
            this.lb_myname.Location = new System.Drawing.Point(14, 49);
            this.lb_myname.Name = "lb_myname";
            this.lb_myname.Size = new System.Drawing.Size(38, 13);
            this.lb_myname.TabIndex = 30;
            this.lb_myname.Text = "Name:";
            // 
            // lb_rec
            // 
            this.lb_rec.AutoSize = true;
            this.lb_rec.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lb_rec.ForeColor = System.Drawing.Color.Red;
            this.lb_rec.Location = new System.Drawing.Point(712, 101);
            this.lb_rec.Name = "lb_rec";
            this.lb_rec.Size = new System.Drawing.Size(23, 13);
            this.lb_rec.TabIndex = 29;
            this.lb_rec.Text = "....";
            // 
            // tb_recintro
            // 
            this.tb_recintro.BackColor = System.Drawing.SystemColors.Control;
            this.tb_recintro.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.tb_recintro.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.tb_recintro.ForeColor = System.Drawing.Color.Black;
            this.tb_recintro.Location = new System.Drawing.Point(470, 100);
            this.tb_recintro.Multiline = true;
            this.tb_recintro.Name = "tb_recintro";
            this.tb_recintro.Size = new System.Drawing.Size(121, 19);
            this.tb_recintro.TabIndex = 28;
            this.tb_recintro.Text = "record intro audio:";
            // 
            // bt_astop
            // 
            this.bt_astop.BackgroundImage = global::oscardata.Properties.Resources.stopplay;
            this.bt_astop.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bt_astop.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bt_astop.ForeColor = System.Drawing.SystemColors.Control;
            this.bt_astop.Location = new System.Drawing.Point(663, 89);
            this.bt_astop.Name = "bt_astop";
            this.bt_astop.Size = new System.Drawing.Size(32, 36);
            this.bt_astop.TabIndex = 27;
            this.bt_astop.UseVisualStyleBackColor = true;
            this.bt_astop.Click += new System.EventHandler(this.bt_astop_Click);
            // 
            // bt_aplay
            // 
            this.bt_aplay.BackgroundImage = global::oscardata.Properties.Resources.playback;
            this.bt_aplay.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bt_aplay.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bt_aplay.ForeColor = System.Drawing.SystemColors.Control;
            this.bt_aplay.Location = new System.Drawing.Point(625, 89);
            this.bt_aplay.Name = "bt_aplay";
            this.bt_aplay.Size = new System.Drawing.Size(32, 36);
            this.bt_aplay.TabIndex = 26;
            this.bt_aplay.UseVisualStyleBackColor = true;
            this.bt_aplay.Click += new System.EventHandler(this.bt_aplay_Click);
            // 
            // bt_arecord
            // 
            this.bt_arecord.BackgroundImage = global::oscardata.Properties.Resources.record;
            this.bt_arecord.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.bt_arecord.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.bt_arecord.ForeColor = System.Drawing.SystemColors.Control;
            this.bt_arecord.Location = new System.Drawing.Point(587, 89);
            this.bt_arecord.Name = "bt_arecord";
            this.bt_arecord.Size = new System.Drawing.Size(32, 36);
            this.bt_arecord.TabIndex = 25;
            this.bt_arecord.UseVisualStyleBackColor = true;
            this.bt_arecord.Click += new System.EventHandler(this.bt_arecord_Click);
            // 
            // cb_sendIntro
            // 
            this.cb_sendIntro.AutoSize = true;
            this.cb_sendIntro.Checked = true;
            this.cb_sendIntro.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cb_sendIntro.Location = new System.Drawing.Point(207, 104);
            this.cb_sendIntro.Name = "cb_sendIntro";
            this.cb_sendIntro.Size = new System.Drawing.Size(105, 17);
            this.cb_sendIntro.TabIndex = 24;
            this.cb_sendIntro.Text = "Send Intro Audio";
            this.cb_sendIntro.UseVisualStyleBackColor = true;
            // 
            // cb_stampinfo
            // 
            this.cb_stampinfo.AutoSize = true;
            this.cb_stampinfo.Checked = true;
            this.cb_stampinfo.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cb_stampinfo.Location = new System.Drawing.Point(207, 84);
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
            this.tb_info.Size = new System.Drawing.Size(492, 20);
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
            this.cb_announcement.Location = new System.Drawing.Point(589, 60);
            this.cb_announcement.Name = "cb_announcement";
            this.cb_announcement.Size = new System.Drawing.Size(56, 21);
            this.cb_announcement.TabIndex = 19;
            this.cb_announcement.Text = "4";
            // 
            // textBox4
            // 
            this.textBox4.BackColor = System.Drawing.SystemColors.Control;
            this.textBox4.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox4.ForeColor = System.Drawing.Color.Black;
            this.textBox4.Location = new System.Drawing.Point(654, 62);
            this.textBox4.Multiline = true;
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(75, 19);
            this.textBox4.TabIndex = 18;
            this.textBox4.Text = "transmissions";
            // 
            // textBox1
            // 
            this.textBox1.BackColor = System.Drawing.SystemColors.Control;
            this.textBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox1.ForeColor = System.Drawing.Color.Black;
            this.textBox1.Location = new System.Drawing.Point(395, 64);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(196, 19);
            this.textBox1.TabIndex = 17;
            this.textBox1.Text = "send announcement before TX every";
            // 
            // tb_callsign
            // 
            this.tb_callsign.CharacterCasing = System.Windows.Forms.CharacterCasing.Upper;
            this.tb_callsign.Location = new System.Drawing.Point(71, 20);
            this.tb_callsign.Name = "tb_callsign";
            this.tb_callsign.Size = new System.Drawing.Size(104, 20);
            this.tb_callsign.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(14, 23);
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
            this.cb_stampcall.Location = new System.Drawing.Point(207, 64);
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
            this.tabPage_about.Size = new System.Drawing.Size(1280, 552);
            this.tabPage_about.TabIndex = 6;
            this.tabPage_about.Text = "About";
            this.tabPage_about.UseVisualStyleBackColor = true;
            // 
            // richTextBox1
            // 
            this.richTextBox1.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.richTextBox1.Location = new System.Drawing.Point(17, 22);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(573, 527);
            this.richTextBox1.TabIndex = 0;
            this.richTextBox1.Text = resources.GetString("richTextBox1.Text");
            // 
            // cb_speed
            // 
            this.cb_speed.FormattingEnabled = true;
            this.cb_speed.Items.AddRange(new object[] {
            "1200 BPSK BW: 1300 Hz",
            "2400 BPSK BW: 2500 Hz",
            "3000 QPSK BW: 1700 Hz ",
            "4000 QPSK BW: 2400 Hz ",
            "4410 QPSK BW: 2500 Hz (QO-100 Standard)",
            "4800 QPSK BW: 2700 Hz",
            "5500 8APSK BW: 2300 Hz",
            "6000 8APSK BW: 2500 Hz (QO-100 Transceiver)",
            "6600 8APSK BW: 2600 Hz",
            "7200 8APSK BW: 2700 Hz (QO-100 SDR)",
            "45.45 Baud RTTY"});
            this.cb_speed.Location = new System.Drawing.Point(122, 2);
            this.cb_speed.Name = "cb_speed";
            this.cb_speed.Size = new System.Drawing.Size(304, 21);
            this.cb_speed.TabIndex = 11;
            this.cb_speed.Text = "4410 QPSK BW: 2500 Hz (QO-100)";
            this.cb_speed.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // label_speed
            // 
            this.label_speed.AutoSize = true;
            this.label_speed.Location = new System.Drawing.Point(31, 5);
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
            // label_fifo
            // 
            this.label_fifo.AutoSize = true;
            this.label_fifo.Location = new System.Drawing.Point(31, 31);
            this.label_fifo.Name = "label_fifo";
            this.label_fifo.Size = new System.Drawing.Size(55, 13);
            this.label_fifo.TabIndex = 14;
            this.label_fifo.Text = "TX Buffer:";
            // 
            // label_capfifo
            // 
            this.label_capfifo.AutoSize = true;
            this.label_capfifo.Location = new System.Drawing.Point(31, 54);
            this.label_capfifo.Name = "label_capfifo";
            this.label_capfifo.Size = new System.Drawing.Size(56, 13);
            this.label_capfifo.TabIndex = 16;
            this.label_capfifo.Text = "RX Buffer:";
            // 
            // lb_rxsignal
            // 
            this.lb_rxsignal.AutoSize = true;
            this.lb_rxsignal.Location = new System.Drawing.Point(448, 54);
            this.lb_rxsignal.Name = "lb_rxsignal";
            this.lb_rxsignal.Size = new System.Drawing.Size(57, 13);
            this.lb_rxsignal.TabIndex = 18;
            this.lb_rxsignal.Text = "RX Signal:";
            // 
            // lb_rxsync
            // 
            this.lb_rxsync.AutoSize = true;
            this.lb_rxsync.Location = new System.Drawing.Point(448, 7);
            this.lb_rxsync.Name = "lb_rxsync";
            this.lb_rxsync.Size = new System.Drawing.Size(52, 13);
            this.lb_rxsync.TabIndex = 20;
            this.lb_rxsync.Text = "RX Sync:";
            // 
            // pn1
            // 
            this.pn1.Controls.Add(this.progressBar_fifo);
            this.pn1.Controls.Add(this.label_capfifo);
            this.pn1.Controls.Add(this.label_speed);
            this.pn1.Controls.Add(this.pb_rxsignal);
            this.pn1.Controls.Add(this.lb_rxsync);
            this.pn1.Controls.Add(this.progressBar_capfifo);
            this.pn1.Controls.Add(this.cb_speed);
            this.pn1.Controls.Add(this.lb_rxsignal);
            this.pn1.Controls.Add(this.pb_rxsync);
            this.pn1.Controls.Add(this.label_fifo);
            this.pn1.Location = new System.Drawing.Point(540, 588);
            this.pn1.Name = "pn1";
            this.pn1.Size = new System.Drawing.Size(538, 74);
            this.pn1.TabIndex = 30;
            // 
            // pb_rxsignal
            // 
            this.pb_rxsignal.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pb_rxsignal.BackgroundImage")));
            this.pb_rxsignal.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_rxsignal.Location = new System.Drawing.Point(511, 47);
            this.pb_rxsignal.Name = "pb_rxsignal";
            this.pb_rxsignal.Size = new System.Drawing.Size(24, 24);
            this.pb_rxsignal.TabIndex = 17;
            this.pb_rxsignal.TabStop = false;
            // 
            // pb_rxsync
            // 
            this.pb_rxsync.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("pb_rxsync.BackgroundImage")));
            this.pb_rxsync.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Zoom;
            this.pb_rxsync.Location = new System.Drawing.Point(511, 2);
            this.pb_rxsync.Name = "pb_rxsync";
            this.pb_rxsync.Size = new System.Drawing.Size(24, 24);
            this.pb_rxsync.TabIndex = 19;
            this.pb_rxsync.TabStop = false;
            // 
            // cb_rx_autosync
            // 
            this.cb_rx_autosync.AutoSize = true;
            this.cb_rx_autosync.Location = new System.Drawing.Point(195, 146);
            this.cb_rx_autosync.Name = "cb_rx_autosync";
            this.cb_rx_autosync.Size = new System.Drawing.Size(93, 17);
            this.cb_rx_autosync.TabIndex = 34;
            this.cb_rx_autosync.Text = "RX Auto Sync";
            this.cb_rx_autosync.UseVisualStyleBackColor = true;
            // 
            // textBox6
            // 
            this.textBox6.BackColor = System.Drawing.SystemColors.Control;
            this.textBox6.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.textBox6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.textBox6.Location = new System.Drawing.Point(195, 167);
            this.textBox6.Multiline = true;
            this.textBox6.Name = "textBox6";
            this.textBox6.ReadOnly = true;
            this.textBox6.Size = new System.Drawing.Size(93, 35);
            this.textBox6.TabIndex = 35;
            this.textBox6.Text = "or double click in spectrum";
            // 
            // progressBar_fifo
            // 
            this.progressBar_fifo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(128)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.progressBar_fifo.Location = new System.Drawing.Point(122, 29);
            this.progressBar_fifo.Maximum = 20;
            this.progressBar_fifo.Name = "progressBar_fifo";
            this.progressBar_fifo.Size = new System.Drawing.Size(304, 18);
            this.progressBar_fifo.Step = 11;
            this.progressBar_fifo.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar_fifo.TabIndex = 13;
            // 
            // progressBar_capfifo
            // 
            this.progressBar_capfifo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(192)))), ((int)(((byte)(128)))));
            this.progressBar_capfifo.Location = new System.Drawing.Point(122, 53);
            this.progressBar_capfifo.Name = "progressBar_capfifo";
            this.progressBar_capfifo.Size = new System.Drawing.Size(304, 18);
            this.progressBar_capfifo.Step = 1;
            this.progressBar_capfifo.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            this.progressBar_capfifo.TabIndex = 15;
            // 
            // vu_cap
            // 
            this.vu_cap.Location = new System.Drawing.Point(479, 111);
            this.vu_cap.Name = "vu_cap";
            this.vu_cap.Size = new System.Drawing.Size(100, 10);
            this.vu_cap.TabIndex = 20;
            // 
            // vu_pb
            // 
            this.vu_pb.Location = new System.Drawing.Point(479, 47);
            this.vu_pb.Name = "vu_pb";
            this.vu_pb.Size = new System.Drawing.Size(100, 10);
            this.vu_pb.TabIndex = 19;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(1296, 693);
            this.Controls.Add(this.pn1);
            this.Controls.Add(this.panel_txspectrum);
            this.Controls.Add(this.panel_constel);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.tabControl1);
            this.ForeColor = System.Drawing.SystemColors.ControlText;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "AMSAT-DL Multimedia HS Modem V0.64 by DJ0ABR";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.tabPage_ber.ResumeLayout(false);
            this.tabPage_ber.PerformLayout();
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
            this.tabPage_rtty.ResumeLayout(false);
            this.tabPage_rtty.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.tabPage_setup.ResumeLayout(false);
            this.tabPage_setup.PerformLayout();
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_audioCAPstatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_audioPBstatus)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_CAPvol)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb_PBvol)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.tabPage_about.ResumeLayout(false);
            this.pn1.ResumeLayout(false);
            this.pn1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pb_rxsignal)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pb_rxsync)).EndInit();
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
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TabPage tabPage_setup;
        private System.Windows.Forms.TextBox tb_callsign;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.CheckBox cb_stampcall;
        private System.Windows.Forms.TextBox tb_shutdown;
        private System.Windows.Forms.Button bt_shutdown;
        private System.Windows.Forms.Button bt_resetmodem;
        private System.Windows.Forms.ComboBox cb_audioCAP;
        private System.Windows.Forms.ComboBox cb_audioPB;
        private System.Windows.Forms.GroupBox groupBox4;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.TrackBar tb_PBvol;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TrackBar tb_CAPvol;
        private KmProgressBar progressBar_fifo;
        private System.Windows.Forms.Label label_fifo;
        private System.Windows.Forms.CheckBox cb_autostart;
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
        private KmProgressBar progressBar_capfifo;
        private System.Windows.Forms.Label label_capfifo;
        private System.Windows.Forms.PictureBox pb_rxsignal;
        private System.Windows.Forms.Label lb_rxsignal;
        private System.Windows.Forms.Label lb_rxsync;
        private System.Windows.Forms.PictureBox pb_rxsync;
        private System.Windows.Forms.ComboBox cb_language;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.ComboBox cb_safemode;
        private KmProgressBar vu_cap;
        private KmProgressBar vu_pb;
        private System.Windows.Forms.ToolStripStatusLabel toolStrip_Type;
        private System.Windows.Forms.CheckBox cb_sendIntro;
        private System.Windows.Forms.Button bt_arecord;
        private System.Windows.Forms.Button bt_astop;
        private System.Windows.Forms.Button bt_aplay;
        private System.Windows.Forms.TextBox tb_recintro;
        private System.Windows.Forms.Label lb_rec;
        private System.Windows.Forms.Label lb_tuningqrgs;
        private System.Windows.Forms.Button bt_allf;
        private System.Windows.Forms.CheckBox cb_marker;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.PictureBox pictureBox4;
        private System.Windows.Forms.Button bt_tune_minus;
        private System.Windows.Forms.Button bt_tune_plus;
        private System.Windows.Forms.TabPage tabPage_rtty;
        private System.Windows.Forms.TextBox tb_rtty_TX;
        private System.Windows.Forms.Button bt_rtty_station;
        private System.Windows.Forms.Button bt_rtty_myinfo;
        private System.Windows.Forms.Button bt_rtty_endqso;
        private System.Windows.Forms.Button bt_rtty_end;
        private System.Windows.Forms.Button bt_rtty_start;
        private System.Windows.Forms.Button bt_rtty_answerCQ;
        private System.Windows.Forms.Button bt_rtty_cq;
        private System.Windows.Forms.TextBox tb_rtty_deftext;
        private System.Windows.Forms.Button bt_rtty_tx;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Label label_urname;
        private System.Windows.Forms.TextBox tb_urname;
        private System.Windows.Forms.Label label_urcall;
        private System.Windows.Forms.TextBox tb_urcall;
        private System.Windows.Forms.Button bt_rtty_default;
        private System.Windows.Forms.RadioButton rb_rtty_edit;
        private System.Windows.Forms.RadioButton rb_rtty_normal;
        private System.Windows.Forms.TextBox tb_rtty_RX;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox tb_myqthloc;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.TextBox tb_myqth;
        private System.Windows.Forms.Label lb_qth;
        private System.Windows.Forms.TextBox tb_myname;
        private System.Windows.Forms.Label lb_myname;
        private System.Windows.Forms.Label label_cfgpath_tit;
        private System.Windows.Forms.Label label_cfgpath;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button bt_rxfont;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button bt_rtty_RY;
        private System.Windows.Forms.Panel pn1;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button bt_rtty_text6;
        private System.Windows.Forms.Button bt_rtty_text5;
        private System.Windows.Forms.Button bt_rtty_text4;
        private System.Windows.Forms.Button bt_rtty_text3;
        private System.Windows.Forms.Button bt_rtty_text2;
        private System.Windows.Forms.Button bt_rtty_text1;
        private System.Windows.Forms.RadioButton rb_rtty_real;
        private System.Windows.Forms.CheckBox cb_rx_autosync;
        private System.Windows.Forms.TextBox textBox6;
    }
}


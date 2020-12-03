namespace oscardata
{
    partial class Form2_showtext
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
            this.rtb = new System.Windows.Forms.RichTextBox();
            this.bt_OK = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // rtb
            // 
            this.rtb.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.rtb.Location = new System.Drawing.Point(13, 13);
            this.rtb.Name = "rtb";
            this.rtb.Size = new System.Drawing.Size(310, 315);
            this.rtb.TabIndex = 0;
            this.rtb.Text = "";
            // 
            // bt_OK
            // 
            this.bt_OK.Location = new System.Drawing.Point(247, 335);
            this.bt_OK.Name = "bt_OK";
            this.bt_OK.Size = new System.Drawing.Size(75, 23);
            this.bt_OK.TabIndex = 1;
            this.bt_OK.Text = "OK";
            this.bt_OK.UseVisualStyleBackColor = true;
            this.bt_OK.Click += new System.EventHandler(this.bt_OK_Click);
            // 
            // Form2_showtext
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(335, 367);
            this.Controls.Add(this.bt_OK);
            this.Controls.Add(this.rtb);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form2_showtext";
            this.ShowIcon = false;
            this.Text = "Form2_showtext";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox rtb;
        private System.Windows.Forms.Button bt_OK;
    }
}
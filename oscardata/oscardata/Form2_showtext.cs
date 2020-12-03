using System;
using System.Drawing;
using System.Windows.Forms;

namespace oscardata
{
    public partial class Form2_showtext : Form
    {
        public Form2_showtext(String title,String s)
        {
            InitializeComponent();
            this.Text = title;
            PrintText(rtb,s);
        }

        void PrintText(RichTextBox rtb, String s)
        {
            AppendTextOnce(rtb, new Font("Courier New", (float)8), Color.Blue, Color.White, s);
        }

        void AppendTextOnce(RichTextBox rtb, Font selfont, Color color, Color bcolor, string text)
        {
            try
            {
                if (text.Contains("\n"))
                {
                    char[] ca = new char[] { '\n', '\r' };

                    text = text.Trim(ca);
                    text += "\n";
                }

                // max. xxx Zeilen, wenn mehr dann lösche älteste
                if (rtb.Lines.Length > 200)
                {
                    rtb.SelectionStart = 0;
                    rtb.SelectionLength = rtb.Text.IndexOf("\n", 0) + 1;
                    rtb.SelectedText = "";
                }

                int start = rtb.TextLength;
                rtb.AppendText(text);
                int end = rtb.TextLength;

                // Textbox may transform chars, so (end-start) != text.Length
                rtb.Select(start, end - start);
                rtb.SelectionColor = color;
                rtb.SelectionFont = selfont;
                rtb.SelectionBackColor = bcolor;
                rtb.Select(end, 0);

                rtb.ScrollToCaret();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
            }
        }

        private void bt_OK_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}

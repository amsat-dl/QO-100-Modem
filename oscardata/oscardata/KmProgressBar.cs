using System;
using System.Drawing;
using System.Windows.Forms;
 
namespace oscardata
{
    class KmProgressBar : ProgressBar
    {
        double myvalue = 0;
        public KmProgressBar()
        {
            SetStyle(ControlStyles.UserPaint, true);
            SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
        }

        protected override void OnPaintBackground(PaintEventArgs pevent)
        {
            // None... Helps control the flicker.
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            try
            {
                Rectangle rect = ClientRectangle;
                Graphics g = e.Graphics;
                Brush br = new SolidBrush(Color.FromArgb(255, ForeColor.R, ForeColor.G, ForeColor.B));

                g.FillRectangle(Brushes.LightGray, rect);
                rect.Inflate(-1, -1);
                g.FillRectangle(Brushes.White, rect);
                rect.Inflate(-1, -1);

                double diff = Math.Abs(Value - myvalue);
                if (Value > myvalue) myvalue += diff/10;
                if (Value < myvalue) myvalue -= diff / 10;

                //myvalue = Value;

                if (myvalue > 0)
                {
                    // As we doing this ourselves we need to draw the chunks on the progress bar
                    Rectangle clip = new Rectangle(rect.X, rect.Y, (int)Math.Round(((double)myvalue / Maximum) * rect.Width), rect.Height);
                    g.FillRectangle(br,clip);
                }
            }
            catch { }
        }
    }
}

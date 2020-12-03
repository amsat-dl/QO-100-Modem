using System;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Windows.Forms;

namespace oscardata
{
    public class Imagehandler
    {
        // Save the file with a specific compression level.
        private void SaveJpg(Image image, string file_name, long compression)
        {
            try
            {
                EncoderParameters encoder_params = new EncoderParameters(1);
                encoder_params.Param[0] = new EncoderParameter(System.Drawing.Imaging.Encoder.Quality, compression);

                ImageCodecInfo image_codec_info = GetEncoderInfo("image/jpeg");
                statics.FileDelete(file_name);
                image.Save(file_name, image_codec_info, encoder_params);
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error saving file '" + file_name +
                    "'\nTry a different file name.\n" + ex.Message,
                    "Save Error", MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
            }
        }

        // Return an ImageCodecInfo object for this mime type.
        private ImageCodecInfo GetEncoderInfo(string mime_type)
        {
            ImageCodecInfo[] encoders = ImageCodecInfo.GetImageEncoders();
            for (int i = 0; i <= encoders.Length; i++)
            {
                if (encoders[i].MimeType == mime_type) return encoders[i];
            }
            return null;
        }

        // Save the file with the indicated maximum file size.
        // Return the compression level used.
        public int SaveJpgAtFileSize(Image image, string file_name, long max_size)
        {
            for (int level = 100; level > 5; level -= 5)
            {
                // Try saving at this compression level.                
                SaveJpg(image, file_name, level);

                // If the file is small enough, we're done.
                if (statics.GetFileSize(file_name) <= max_size)
                    return level;
            }
            return 5;
        }

         public Bitmap ResizeImage(Image image, int width, int height, String callsign, String info)
        {
            // get original size of img
            int x = image.Width;
            int y = image.Height;

            // scale the greater size to the destination size
            double relx = (double)width / (double)x;
            double rely = (double)height / (double)y;
            int nw = (int)((double)x * relx);
            int nh = (int)((double)y * relx);
            if (rely < relx)
            {
                nw = (int)((double)x * rely);
                nh = (int)((double)y * rely);
            }

            Bitmap destImage = new Bitmap(nw, nh);
            using (Graphics g = Graphics.FromImage(destImage))
            {
                g.DrawImage(image, 0, 0, nw, nh);
                if (callsign != "")
                {
                    using (var fnt = new Font("Verdana", 15.0f))
                    {
                        var size = g.MeasureString(callsign, fnt);
                        var rect = new RectangleF(5, 5, size.Width, size.Height);
                        SolidBrush opaqueBrush = new SolidBrush(Color.FromArgb(128, 255, 255, 255));

                        g.FillRectangle(opaqueBrush, rect);
                        g.DrawString(callsign, fnt, Brushes.Blue, 5, 5);
                    }
                }
                if (info != "")
                {
                    using (var fnt = new Font("Verdana", 11.0f))
                    {
                        int ypos = nh - 30;
                        var size = g.MeasureString(info, fnt);
                        var rect = new RectangleF(5, ypos, size.Width, size.Height);
                        SolidBrush opaqueBrush = new SolidBrush(Color.FromArgb(128, 255, 255, 255));

                        g.FillRectangle(opaqueBrush, rect);
                        g.DrawString(info, fnt, Brushes.Blue, 5, ypos);
                    }
                }
            }

            return destImage;
        }
    }
}

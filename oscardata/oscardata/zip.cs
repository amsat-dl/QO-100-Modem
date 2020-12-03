using System;
using System.IO;
using System.IO.Compression;


namespace oscardata
{
    public class ZipStorer
    {
        public String unzipFile(String zipfilename)
        {
            try
            {
                using (var zip = ZipFile.Open(zipfilename, ZipArchiveMode.Read))
                {
                    // create a temporary subfolder (delete if exists)
                    String pth = statics.addTmpPath("modemzip");
                    try
                    {
                        Directory.Delete(pth, true);
                    }
                    catch { }
                    Directory.CreateDirectory(pth);
                    // extract the ZIP into this subfolder
                    zip.ExtractToDirectory(pth);

                    // search file in this folder and move it to: unzipped_RXtempfilename
                    String[] files = Directory.GetFiles(pth);
                    foreach (String s in files)
                    {
                        if (s[0] != '.') // ignore path entries
                        {
                            return s;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }

            return null;
        }

        // zipfilename ... name of xyz.zip file
        // filename ... name of file inside the zip file (with or without path, as required)
        // FullPath ... pathname+filename of the file to be included in the zip
        // returns: size of zip file
        public long zipFile(String zipfilename, String filename, String FullPath)
        {
            try
            {
                statics.FileDelete(zipfilename);
            }
            catch { }

            try
            {
                using (var zip = ZipFile.Open(zipfilename, ZipArchiveMode.Create))
                {
                    zip.CreateEntryFromFile(FullPath, filename);
                }

                return statics.GetFileSize(zipfilename);
            }
            catch { }
            return 0;
        }
    }
}

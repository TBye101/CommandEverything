using ICSharpCode.SharpZipLib.Checksums;
using ICSharpCode.SharpZipLib.Core;
using ICSharpCode.SharpZipLib.Zip;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CommandEverything.Framework.Util
{
    /// <summary>
    /// Does some file zipping stuff.
    /// </summary>
    public static class ZipUtils
    {

        /// <summary>
        /// Compresses the files in the nominated folder, and creates a zip file on disk named as outPathname.
        /// </summary>
        /// <param name="outPathname"></param>
        /// <param name="password"></param>
        /// <param name="folderName"></param>
        public static void ZipFolder(string outPathname, string folderName, string password = null)
        {

            FileStream fsOut = File.Create(outPathname);
            ZipOutputStream zipStream = new ZipOutputStream(fsOut);

            zipStream.SetLevel(9); //0-9, 9 being the highest level of compression

            zipStream.Password = password;  // optional. Null is the same as not setting. Required if using AES.

            // This setting will strip the leading part of the folder path in the entries, to
            // make the entries relative to the starting folder.
            // To include the full path for each entry up to the drive root, assign folderOffset = 0.
            int folderOffset = folderName.Length + (folderName.EndsWith("\\") ? 0 : 1);

            ZipFolder(folderName, zipStream, folderOffset);

            zipStream.IsStreamOwner = true; // Makes the Close also Close the underlying stream
            zipStream.Close();
        }

        /// <summary>
        /// Zips all files in the specified directory.
        /// </summary>
        /// <param name="path">The specified directory.</param>
        /// <param name="zipStream"></param>
        /// <param name="folderOffset"></param>
        public static void ZipFolder(string path, ZipOutputStream zipStream, int folderOffset)
        {

            string[] files = Directory.GetFiles(path);

            foreach (string filename in files)
            {

                FileInfo fi = new FileInfo(filename);

                string entryName = filename.Substring(folderOffset); // Makes the name in zip based on the folder
                entryName = ZipEntry.CleanName(entryName); // Removes drive from name and fixes slash direction
                ZipEntry newEntry = new ZipEntry(entryName);
                newEntry.DateTime = fi.LastWriteTime; // Note the zip format stores 2 second granularity

                // Specifying the AESKeySize triggers AES encryption. Allowable values are 0 (off), 128 or 256.
                // A password on the ZipOutputStream is required if using AES.
                //   newEntry.AESKeySize = 256;

                // To permit the zip to be unpacked by built-in extractor in WinXP and Server2003, WinZip 8, Java, and other older code,
                // you need to do one of the following: Specify UseZip64.Off, or set the Size.
                // If the file may be bigger than 4GB, or you do not need WinXP built-in compatibility, you do not need either,
                // but the zip will be in Zip64 format which not all utilities can understand.
                //   zipStream.UseZip64 = UseZip64.Off;
                newEntry.Size = fi.Length;

                zipStream.PutNextEntry(newEntry);

                // Zip the file in buffered chunks
                // the "using" will close the stream even if an exception occurs
                byte[] buffer = new byte[4096];
                using (FileStream streamReader = File.OpenRead(filename))
                {
                    StreamUtils.Copy(streamReader, zipStream, buffer);
                }
                zipStream.CloseEntry();
            }
            string[] folders = Directory.GetDirectories(path);
            foreach (string folder in folders)
            {
                ZipFolder(folder, zipStream, folderOffset);
            }
        }

        /// <summary>
        /// Writes the zip file.
        /// </summary>
        /// <param name="filesToZip">The files to zip.</param>
        /// <param name="path">The destination path.</param>
        /// <param name="compression">The compression level.</param>
        public static void ZipFiles(List<string> filesToZip, string path, int compression)
        {
            if (compression < 0 || compression > 9)
            {
                throw new ArgumentException("Invalid compression rate.");
            }

            if (!Directory.Exists(new FileInfo(path).Directory.ToString()))
            {
                throw new ArgumentException("The Path does not exist.");
            }

            foreach (string c in filesToZip)
            {
                if (!File.Exists(c))
                {
                    throw new ArgumentException(string.Format("The File{0}does not exist!", c));
                }
            }


            Crc32 crc32 = new Crc32();
            ZipOutputStream stream = new ZipOutputStream(File.Create(path));
            stream.SetLevel(compression);

            for (int i = 0; i < filesToZip.Count; i++)
            {
                ZipEntry entry = new ZipEntry(Path.GetFileName(filesToZip[i]));
                entry.DateTime = DateTime.Now;

                using (FileStream fs = File.OpenRead(filesToZip[i]))
                {
                    byte[] buffer = new byte[fs.Length];
                    fs.Read(buffer, 0, buffer.Length);
                    entry.Size = fs.Length;
                    fs.Close();
                    crc32.Reset();
                    crc32.Update(buffer);
                    entry.Crc = crc32.Value;
                    stream.PutNextEntry(entry);
                    stream.Write(buffer, 0, buffer.Length);
                }
            }
            stream.Finish();
            stream.Close();
        }


        /// <summary>
        /// Compresses a single file.
        /// </summary>
        /// <param name="inputPath"></param>
        /// <param name="outputPath"></param>
        public static void ZipFile(string inputPath, string outputPath)
        {
            FileInfo outFileInfo = new FileInfo(outputPath);
            FileInfo inFileInfo = new FileInfo(inputPath);

            // Create the output directory if it does not exist
            if (!Directory.Exists(outFileInfo.Directory.FullName))
            {
                Directory.CreateDirectory(outFileInfo.Directory.FullName);
            }

            // Compress
            using (FileStream fsOut = File.Create(outputPath))
            {
                using (ZipOutputStream zipStream = new ZipOutputStream(fsOut))
                {
                    zipStream.SetLevel(3);

                    ZipEntry newEntry = new ZipEntry(inFileInfo.Name);
                    newEntry.DateTime = DateTime.UtcNow;
                    zipStream.PutNextEntry(newEntry);

                    byte[] buffer = new byte[4096];
                    using (FileStream streamReader = File.OpenRead(inputPath))
                    {
                        StreamUtils.Copy(streamReader, zipStream, buffer);
                    }

                    zipStream.CloseEntry();
                    zipStream.IsStreamOwner = true;
                    zipStream.Close();
                }
            }
        }
    }
}

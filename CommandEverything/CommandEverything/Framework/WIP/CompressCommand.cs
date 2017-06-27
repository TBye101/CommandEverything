using CommandEverything.Framework.Util;
using CommandEverything.Framework.Util.Text;
using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CommandEverything.Framework.WIP
{
    public class CompressCommand : ICommand
    {
        public string GetHelp()
        {
            return "Used to compress a file.";
        }

        public string GetName()
        {
            return "Compress File";
        }

        public void Run(string Input)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.FileName = "File";
            dlg.DefaultExt = ".txt";

            // Show open file dialog box
            DialogResult result = dlg.ShowDialog();

            // Process open file dialog box results
            if (result == DialogResult.OK)
            {
                // Open document
                string[] file = File.ReadAllLines(dlg.FileName, Encoding.Default);
                ConsoleWriter.WriteLine("Opened file");
                string compressed = CompressString(Utility.ConvertArray(file));
                ConsoleWriter.WriteLine("Compressed file");

                SaveFileDialog sv = new SaveFileDialog();
                sv.FileName = dlg.FileName;
                sv.DefaultExt = ".ce";

                DialogResult res = sv.ShowDialog();

                if (res == DialogResult.OK)
                {
                    StreamWriter sw = new StreamWriter(sv.FileName);
                    sw.WriteLine(compressed);
                    sw.Flush();
                    sw.Dispose();
                    ConsoleWriter.WriteLine("Compressed file saved");
                }
            }
            else
            {
                ConsoleWriter.WriteLine("File invalid!");
            }
        }

        public bool ShouldRunThisCommand(string Input)
        {
            string[] Valid = { "compress", "compress file", "shrink file"};
            return Utility.DoesStringContain(Input, Valid);
        }

        /// <summary>
        /// Compresses the string.
        /// </summary>
        /// <param name="text">The text.</param>
        /// <returns></returns>
        private string CompressString(string text)
        {
            byte[] buffer = Encoding.UTF8.GetBytes(text);
            var memoryStream = new MemoryStream();
            using (var gZipStream = new GZipStream(memoryStream, CompressionMode.Compress, true))
            {
                gZipStream.Write(buffer, 0, buffer.Length);
            }

            memoryStream.Position = 0;

            var compressedData = new byte[memoryStream.Length];
            memoryStream.Read(compressedData, 0, compressedData.Length);

            var gZipBuffer = new byte[compressedData.Length + 4];
            Buffer.BlockCopy(compressedData, 0, gZipBuffer, 4, compressedData.Length);
            Buffer.BlockCopy(BitConverter.GetBytes(buffer.Length), 0, gZipBuffer, 0, 4);
            return Convert.ToBase64String(gZipBuffer);
        }

        /// <summary>
        /// Decompresses the string.
        /// </summary>
        /// <param name="compressedText">The compressed text.</param>
        /// <returns></returns>
        private string DecompressString(string compressedText)
        {
            byte[] gZipBuffer = Convert.FromBase64String(compressedText);
            using (var memoryStream = new MemoryStream())
            {
                int dataLength = BitConverter.ToInt32(gZipBuffer, 0);
                memoryStream.Write(gZipBuffer, 4, gZipBuffer.Length - 4);

                var buffer = new byte[dataLength];

                memoryStream.Position = 0;
                using (var gZipStream = new GZipStream(memoryStream, CompressionMode.Decompress))
                {
                    gZipStream.Read(buffer, 0, buffer.Length);
                }

                return Encoding.UTF8.GetString(buffer);
            }
        }
    }
}

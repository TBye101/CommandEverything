using CommandEverything.Framework.Util;
using CommandEverything.Framework.Util.Text;
using DataStorage.Compression.CompressionA;
using System;
using System.Collections.Generic;
using System.IO;
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
            dlg.FileName = "File"; // Default file name
            dlg.DefaultExt = ".txt"; // Default file extension

            // Show open file dialog box
            DialogResult result = dlg.ShowDialog();

            // Process open file dialog box results
            if (result == DialogResult.OK)
            {
                // Open document
                string[] file = File.ReadAllLines(dlg.FileName, Encoding.Default);
                ConsoleWriter.WriteLine("Opened file");
                string compressed = CompressA.Compress(Utility.ConvertArray(file));
                ConsoleWriter.WriteLine("Compressed file");

                SaveFileDialog sv = new SaveFileDialog();
                sv.FileName = dlg.FileName;
                sv.DefaultExt = "ce";
                DialogResult res = sv.ShowDialog();

                if (res == DialogResult.OK)
                {
                    StreamWriter sw = new StreamWriter(sv.FileName);
                    sw.WriteLine(compressed);
                    sw.Flush();
                    sw.Dispose();
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
    }
}

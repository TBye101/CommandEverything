using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataStorage.Compression.CompressionA
{
    /// <summary>
    /// Used to compress and decompress strings.
    /// </summary>
    public static class CompressA
    {
        /// <summary>
        /// Stores the current compression dictionary.
        /// </summary>
        private static Dictionary<string, string> CompressionDictionary = DictionaryParser.ParseFile(Resources.CompressionDictionaryA.ToString());

        /// <summary>
        /// Compresses the given string and returns it.
        /// </summary>
        /// <param name="ToCompress"></param>
        /// <returns></returns>
        public static string Compress(string ToCompress)
        {
            string Binary = Encoding.ASCII.GetBytes(ToCompress).ToString();

            int i = 0;
            int size = CompressionDictionary.Count;

            while (i != size)
            {
                string item = CompressionDictionary.ElementAt(size - 1).Key;
                //search for it and replace it.
                i++;
            }

            foreach (string item in CompressionDictionary.ElementAt Reverse())
            {

            }
            //Write compression logic here
        }

        public static string Decompress(string ToDecompress)
        {

        }
    }
}
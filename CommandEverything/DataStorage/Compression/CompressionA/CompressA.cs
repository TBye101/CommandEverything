using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DataStorage.Compression.CompressionA
{
    /// <summary>
    /// Used to compress strings.
    /// </summary>
    public static class CompressA
    {
        /// <summary>
        /// Stores the current compression dictionary.
        /// </summary>
        private static Dictionary<string, string> CompressionDictionary = new Dictionary<string, string>();

        /// <summary>
        /// Compresses the given string and returns it.
        /// </summary>
        /// <param name="ToCompress"></param>
        /// <returns></returns>
        public static string Compress(string ToCompress)
        {
            string Binary = Encoding.ASCII.GetBytes(ToCompress).ToString();

        }
    }
}
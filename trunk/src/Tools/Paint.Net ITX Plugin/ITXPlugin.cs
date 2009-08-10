using System;
using System.Collections.Generic;
using System.Text;
using PaintDotNet;
using PaintDotNet.Data;
using System.IO;
using System.Drawing;
using System.Windows.Forms;
using System.Drawing.Imaging;
using LibITX;

namespace ITXPlugin
{
    public class MyFileType : FileType
    {
        public MyFileType()
            : base("Initial Texture ITX",
                FileTypeFlags.SupportsLoading | FileTypeFlags.SupportsSaving,
                new String[] { ".itx" })
        {
        }

        protected override Document OnLoad(Stream input)
        {
            try
            {
                Bitmap b = new Bitmap(500, 500);
                ITXImage image;
                //image.Load("test");

                return Document.FromImage(b);
            }
            catch
            {
                MessageBox.Show("Problem Importing File");

                Bitmap b = new Bitmap(500, 500);
                return Document.FromImage(b);
            }
        }

        protected override void OnSave(Document input, Stream output, SaveConfigToken token, 
            Surface scratchSurface, ProgressEventHandler callback)
        {
            RenderArgs ra = new RenderArgs(new Surface(input.Size));
            input.Render(ra);

            ra.Bitmap.Save(output, ImageFormat.Bmp);
        }
    }

    public class MyFileTypeFactory : IFileTypeFactory
    {
        public FileType[] GetFileTypeInstances()
        {
            return new FileType[] { new MyFileType() };
        }
    }
}

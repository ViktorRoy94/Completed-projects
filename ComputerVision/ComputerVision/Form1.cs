using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ComputerVision
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void openFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
			if (dialog.ShowDialog() == DialogResult.OK)
			{
				Bitmap image = new Bitmap(dialog.FileName);
				pictureBox1.Image = (Image)image;
				pictureBox1.BorderStyle = BorderStyle.FixedSingle;
				pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
			}
        }

		private void exitToolStripMenuItem_Click(object sender, EventArgs e)
		{
			Application.Exit();
		}

		private void saveFileToolStripMenuItem_Click(object sender, EventArgs e)
		{
			SaveFileDialog dialog = new SaveFileDialog();
			dialog.Filter = "JPeg Image|*.jpg |All files (*.*)|*.*";
			dialog.Title = "Save an Image File";
			dialog.ShowDialog();
			if (dialog.FileName != "")
			{
				System.IO.FileStream fs = (System.IO.FileStream)dialog.OpenFile();
				pictureBox1.Image.Save(fs, System.Drawing.Imaging.ImageFormat.Jpeg);
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
			int r, g, b;
			int c, m, y, k;
			r = Convert.ToInt32(textBox1.Text);
			g = Convert.ToInt32(textBox2.Text);
			b = Convert.ToInt32(textBox7.Text);
			k = min(r, g, b);
			c = 255 - r;
			m = 255 - g;
			y = 255 - b;
			textBox4.Text = Convert.ToString(c);
			textBox3.Text = Convert.ToString(m);
			textBox8.Text = Convert.ToString(y);

		}
		private int min(int r, int g, int b)
		{
			if ((r < g) && (r < b)) return r;
			else
				if ((g < r) && (g < b)) return g;
				else
					if ((b < r) && (b < g)) return b;

			return 0;
		}

		private void button2_Click(object sender, EventArgs e)
		{
			int r, g, b;
			int c, m, y;
			c = Convert.ToInt32(textBox4.Text);
			m = Convert.ToInt32(textBox3.Text);
			y = Convert.ToInt32(textBox8.Text);
			r = 255 - c;
			g = 255 - m;
			b = 255 - y;
			textBox1.Text = Convert.ToString(r);
			textBox2.Text = Convert.ToString(g);
			textBox7.Text = Convert.ToString(b);
		}

		private void button4_Click(object sender, EventArgs e)
		{
			double r, g, b;
			double x,y,z;
			r = Convert.ToInt32(textBox1.Text);
			g = Convert.ToInt32(textBox2.Text);
			b = Convert.ToInt32(textBox7.Text);
			r = r / 255;
			g = g / 255;
			b = b / 255;
			x = 0.6068909 * r + 0.1735011 * g + 0.2003480 * b;
			y = 0.2989164 * r + 0.5865990 * g + 0.1144845 * b;
			z = 0.0000000 * r + 0.0660957 * g + 1.1162243 * b;
			if ((r == 1) && (g == 1) && (b == 1))
			{
				z = 1;
			}
			textBox6.Text = Convert.ToString(x);
			textBox5.Text = Convert.ToString(y);
			textBox9.Text = Convert.ToString(z);

		}

		private void button3_Click(object sender, EventArgs e)
		{
			int r, g, b;
			double x, y, z;
			x = Convert.ToDouble(textBox6.Text);
			y = Convert.ToDouble(textBox5.Text);
			z = Convert.ToDouble(textBox9.Text);
			r = (int)(1.9099961 * x - 0.5324542 * y - 0.2882091 * z);
			g = (int)(-0.9846663 * x + 1.9991710 * y - 0.0283082 * z);
			b = (int)(0.0583056 * x - 0.1183781 * y + 0.8975535 * z);
			textBox1.Text = Convert.ToString(r);
			textBox2.Text = Convert.ToString(g);
			textBox7.Text = Convert.ToString(b);

		}

		private void button6_Click(object sender, EventArgs e)
		{
			int c, m, y1;
			double x, y, z;
			c = Convert.ToInt32(textBox4.Text);
			m = Convert.ToInt32(textBox3.Text);
			y1 = Convert.ToInt32(textBox8.Text);
			x = 0.6068909 * c + 0.1735011 * m + 0.2003480 * y1;
			y = 0.2989164 * c + 0.5865990 * m + 0.1144845 * y1;
			z = 0.0000000 * c + 0.0660957 * m + 1.1162243 * y1;
			textBox6.Text = Convert.ToString(x);
			textBox5.Text = Convert.ToString(y);
			textBox9.Text = Convert.ToString(z);
		}

		private void button5_Click(object sender, EventArgs e)
		{
			int c, m, y1;
			double x, y, z;
			x = Convert.ToDouble(textBox6.Text);
			y = Convert.ToDouble(textBox5.Text);
			z = Convert.ToDouble(textBox9.Text);
			c =255-(int)(1.9099961 * x - 0.5324542 * y - 0.2882091 * z);
			m =255-(int)(-0.9846663 * x + 1.9991710 * y - 0.0283082 * z);
			y1 =255-(int)(0.0583056 * x - 0.1183781 * y + 0.8975535 * z);
			textBox4.Text = Convert.ToString(c);
			textBox3.Text = Convert.ToString(m);
			textBox8.Text = Convert.ToString(y1);

		}
    }
}

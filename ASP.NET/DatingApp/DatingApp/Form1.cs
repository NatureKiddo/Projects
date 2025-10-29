using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace DatingApp
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void btnapply_Click(object sender, EventArgs e)
        {
            string first = fname.Text;
            string last = lname.Text;
            int Age = int.Parse(age1.Text);
            string Gender = gender1.Text;
            int age = int.Parse(age2.Text);
            string gender = gender2.Text;

            string message = "Welcome, " + first + " " + last + "(" + Age + ") " + Gender + ", seeking a " + gender + "(" + age + ")";
            MessageBox.Show(message);
        }

        private void textBox4_TextChanged(object sender, EventArgs e)
        {

        }
    }
}

namespace DatingApp
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.fname1 = new System.Windows.Forms.Label();
            this.btnapply = new System.Windows.Forms.Button();
            this.gender1 = new System.Windows.Forms.ComboBox();
            this.btncancel = new System.Windows.Forms.Button();
            this.fname = new System.Windows.Forms.TextBox();
            this.lname = new System.Windows.Forms.TextBox();
            this.age1 = new System.Windows.Forms.TextBox();
            this.lname1 = new System.Windows.Forms.Label();
            this.age0 = new System.Windows.Forms.Label();
            this.gender0 = new System.Windows.Forms.Label();
            this.age4 = new System.Windows.Forms.Label();
            this.gender3 = new System.Windows.Forms.Label();
            this.age2 = new System.Windows.Forms.TextBox();
            this.gender2 = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.gender0);
            this.groupBox1.Controls.Add(this.age0);
            this.groupBox1.Controls.Add(this.lname1);
            this.groupBox1.Controls.Add(this.age1);
            this.groupBox1.Controls.Add(this.lname);
            this.groupBox1.Controls.Add(this.fname);
            this.groupBox1.Controls.Add(this.gender1);
            this.groupBox1.Controls.Add(this.fname1);
            this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox1.Location = new System.Drawing.Point(12, 25);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(383, 274);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "About You";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.gender2);
            this.groupBox2.Controls.Add(this.age2);
            this.groupBox2.Controls.Add(this.gender3);
            this.groupBox2.Controls.Add(this.age4);
            this.groupBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.groupBox2.Location = new System.Drawing.Point(419, 25);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(369, 173);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Your Preferred Partner";
            // 
            // fname1
            // 
            this.fname1.AutoSize = true;
            this.fname1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.fname1.Location = new System.Drawing.Point(6, 57);
            this.fname1.Name = "fname1";
            this.fname1.Size = new System.Drawing.Size(105, 20);
            this.fname1.TabIndex = 0;
            this.fname1.Text = "First name:";
            // 
            // btnapply
            // 
            this.btnapply.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnapply.Location = new System.Drawing.Point(419, 232);
            this.btnapply.Name = "btnapply";
            this.btnapply.Size = new System.Drawing.Size(127, 57);
            this.btnapply.TabIndex = 2;
            this.btnapply.Text = "Apply";
            this.btnapply.UseVisualStyleBackColor = true;
            this.btnapply.Click += new System.EventHandler(this.btnapply_Click);
            // 
            // gender1
            // 
            this.gender1.FormattingEnabled = true;
            this.gender1.Items.AddRange(new object[] {
            "Male ",
            "Female ",
            ""});
            this.gender1.Location = new System.Drawing.Point(128, 222);
            this.gender1.Name = "gender1";
            this.gender1.Size = new System.Drawing.Size(214, 28);
            this.gender1.TabIndex = 1;
            // 
            // btncancel
            // 
            this.btncancel.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Italic, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btncancel.Location = new System.Drawing.Point(601, 232);
            this.btncancel.Name = "btncancel";
            this.btncancel.Size = new System.Drawing.Size(139, 57);
            this.btncancel.TabIndex = 3;
            this.btncancel.Text = "Cancel";
            this.btncancel.UseVisualStyleBackColor = true;
            this.btncancel.Click += new System.EventHandler(this.button2_Click);
            // 
            // fname
            // 
            this.fname.Location = new System.Drawing.Point(128, 42);
            this.fname.Multiline = true;
            this.fname.Name = "fname";
            this.fname.Size = new System.Drawing.Size(214, 35);
            this.fname.TabIndex = 2;
            // 
            // lname
            // 
            this.lname.Location = new System.Drawing.Point(124, 100);
            this.lname.Multiline = true;
            this.lname.Name = "lname";
            this.lname.Size = new System.Drawing.Size(214, 37);
            this.lname.TabIndex = 3;
            // 
            // age1
            // 
            this.age1.Location = new System.Drawing.Point(124, 152);
            this.age1.Multiline = true;
            this.age1.Name = "age1";
            this.age1.Size = new System.Drawing.Size(214, 40);
            this.age1.TabIndex = 4;
            // 
            // lname1
            // 
            this.lname1.AutoSize = true;
            this.lname1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lname1.Location = new System.Drawing.Point(6, 117);
            this.lname1.Name = "lname1";
            this.lname1.Size = new System.Drawing.Size(103, 20);
            this.lname1.TabIndex = 5;
            this.lname1.Text = "Last name:";
            // 
            // age0
            // 
            this.age0.AutoSize = true;
            this.age0.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.age0.Location = new System.Drawing.Point(61, 172);
            this.age0.Name = "age0";
            this.age0.Size = new System.Drawing.Size(47, 20);
            this.age0.TabIndex = 6;
            this.age0.Text = "Age:";
            // 
            // gender0
            // 
            this.gender0.AutoSize = true;
            this.gender0.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gender0.Location = new System.Drawing.Point(32, 230);
            this.gender0.Name = "gender0";
            this.gender0.Size = new System.Drawing.Size(76, 20);
            this.gender0.TabIndex = 7;
            this.gender0.Text = "Gender:";
            // 
            // age4
            // 
            this.age4.AutoSize = true;
            this.age4.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.age4.Location = new System.Drawing.Point(16, 39);
            this.age4.Name = "age4";
            this.age4.Size = new System.Drawing.Size(47, 20);
            this.age4.TabIndex = 0;
            this.age4.Text = "Age:";
            // 
            // gender3
            // 
            this.gender3.AutoSize = true;
            this.gender3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.gender3.Location = new System.Drawing.Point(16, 100);
            this.gender3.Name = "gender3";
            this.gender3.Size = new System.Drawing.Size(76, 20);
            this.gender3.TabIndex = 1;
            this.gender3.Text = "Gender:";
            // 
            // age2
            // 
            this.age2.Location = new System.Drawing.Point(130, 33);
            this.age2.Multiline = true;
            this.age2.Name = "age2";
            this.age2.Size = new System.Drawing.Size(191, 26);
            this.age2.TabIndex = 2;
            this.age2.TextChanged += new System.EventHandler(this.textBox4_TextChanged);
            // 
            // gender2
            // 
            this.gender2.FormattingEnabled = true;
            this.gender2.Items.AddRange(new object[] {
            "Female",
            "Male"});
            this.gender2.Location = new System.Drawing.Point(130, 103);
            this.gender2.Name = "gender2";
            this.gender2.Size = new System.Drawing.Size(191, 24);
            this.gender2.TabIndex = 3;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 314);
            this.Controls.Add(this.btncancel);
            this.Controls.Add(this.btnapply);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox gender1;
        private System.Windows.Forms.Label fname1;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button btnapply;
        private System.Windows.Forms.Button btncancel;
        private System.Windows.Forms.Label gender0;
        private System.Windows.Forms.Label age0;
        private System.Windows.Forms.Label lname1;
        private System.Windows.Forms.TextBox age1;
        private System.Windows.Forms.TextBox lname;
        private System.Windows.Forms.TextBox fname;
        private System.Windows.Forms.ComboBox gender2;
        private System.Windows.Forms.TextBox age2;
        private System.Windows.Forms.Label gender3;
        private System.Windows.Forms.Label age4;
    }
}


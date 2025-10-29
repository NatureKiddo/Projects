using System;



delegate void ResultEventHandler(object sender, ResultEventArgs e);//delegate is inputed here



class ResultEventArgs : EventArgs
{
    public string Message { get; set; }//a message shall be displayed
}



class StudentReportCard
{
    public string Name { get; set; }//the user name and surname
    public int ComputerScienceMarks { get; set; }//the user mark in computer science
    public int MathMarks { get; set; }//user mark in math
    public int EnglishMarks { get; set; }//user mark in english



    public event ResultEventHandler ResultEvent;



    public void CalculateResult()
    {//the total marks of all three subjects and calculating togther for a total of 150
        int totalMarks = ComputerScienceMarks + MathMarks + EnglishMarks;
        //if statment if the final mark is over 75 then it shall display this message 
        if (totalMarks >= 75)
        {   //Total mark displayed over 150
            OnResultEvent($"Your final mark is {totalMarks} / 150");
            OnResultEvent($"Congratulations {Name}, you have passed the exam!");
        }
        else//if the final mark is not over 75 then it shall display this message
        {
            OnResultEvent($"Your final mark is {totalMarks} / 150");
            OnResultEvent($"Sorry {Name}, you have failed the exam. Your grade is F.");
        }
    }



    protected virtual void OnResultEvent(string message)
    {   //the final result being displayed in a message and with a few words
        ResultEvent?.Invoke(this, new ResultEventArgs { Message = message });
    }
}



class Program
{
    static void Main(string[] args)//main class
    {
        Console.Write("Enter your name: ");//the user enter its name
        string name = Console.ReadLine();



        Console.Write("Enter your Computer Science marks (out of 50): ");//user enter its computer mark
        int csMarks = int.Parse(Console.ReadLine());



        Console.Write("Enter your Math marks (out of 50): ");//user enters its math mark
        int mathMarks = int.Parse(Console.ReadLine());



        Console.Write("Enter your English marks (out of 50): ");//user enters its english mark
        int engMarks = int.Parse(Console.ReadLine());



        StudentReportCard student = new StudentReportCard//on the student report card is what is being displayed
        {
            Name = name,//the name of the user
            ComputerScienceMarks = csMarks,//mark of computer science of the user
            MathMarks = mathMarks,//maths mark of the user
            EnglishMarks = engMarks//the mark of english of its user
        };



        student.ResultEvent += DisplayResultMessage;//result message being displayed



        student.CalculateResult();//calculated resulys



        Console.ReadKey();
    }



    static void DisplayResultMessage(object sender, ResultEventArgs e)
    {
        Console.WriteLine(e.Message);
    }
}
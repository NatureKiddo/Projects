import sqlite3
from datetime import datetime
from tkinter import messagebox
from tkcalendar import DateEntry
from ttkbootstrap import Style
import ttkbootstrap as ttk

# Part 1: Database Setup
class Database:
    """Manages all database interactions."""
    def __init__(self, db_name="library.db"):
        self.conn = sqlite3.connect(db_name)
        self.cursor = self.conn.cursor()
        self.create_tables()
        self.insert_sample_data()

    def create_tables(self):
        """Creates the books and borrowed_books tables if they don't exist."""
        self.cursor.execute('''
            CREATE TABLE IF NOT EXISTS books (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                title TEXT NOT NULL,
                quantity INTEGER NOT NULL
            )
        ''')
        self.cursor.execute('''
            CREATE TABLE IF NOT EXISTS borrowed_books (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                student_name TEXT NOT NULL,
                book_id INTEGER,
                borrow_date TEXT,
                return_date TEXT,
                fine REAL DEFAULT 0,
                FOREIGN KEY(book_id) REFERENCES books(id)
            )
        ''')
        self.conn.commit()

    def insert_sample_data(self):
        """Inserts sample data into the database if the tables are empty."""
        self.cursor.execute("SELECT COUNT(*) FROM books")
        if self.cursor.fetchone()[0] == 0:
            books = [
                ('The Python Book', 5),
                ('Data Science Handbook', 3),
                ('The Great Gatsby', 2),
                ('1984', 1),
                ('Clean Code', 4)
            ]
            self.cursor.executemany("INSERT INTO books (title, quantity) VALUES (?, ?)", books)
            self.conn.commit()
        
        self.cursor.execute("SELECT COUNT(*) FROM borrowed_books")
        if self.cursor.fetchone()[0] == 0:
            borrowed = [
                ('Alice Johnson', 1, '2025-10-01', '2025-10-15', 0),
                ('Bob Williams', 2, '2025-10-05', '2025-10-12', 0),
                ('Charlie Brown', 3, '2025-10-10', '2025-10-16', 50)  # Overdue example
            ]
            self.cursor.executemany("INSERT INTO borrowed_books (student_name, book_id, borrow_date, return_date, fine) VALUES (?, ?, ?, ?, ?)", borrowed)
            self.conn.commit()

    def get_available_books(self):
        """Fetches books with a quantity > 0 for the combobox."""
        self.cursor.execute("SELECT id, title FROM books WHERE quantity > 0")
        return self.cursor.fetchall()

    def get_borrowed_books_view(self, search_term=""):
        """Fetches all borrowed book records with associated book titles."""
        query = '''
            SELECT
                bb.id, bb.student_name, b.title, bb.borrow_date, bb.return_date, bb.fine
            FROM borrowed_books bb
            JOIN books b ON bb.book_id = b.id
            WHERE
                LOWER(bb.student_name) LIKE ? OR LOWER(b.title) LIKE ?
        '''
        self.cursor.execute(query, (f"%{search_term.lower()}%", f"%{search_term.lower()}%"))
        return self.cursor.fetchall()

    def get_book_id_by_title(self, title):
        """Returns the ID of a book based on its title."""
        self.cursor.execute("SELECT id FROM books WHERE title = ?", (title,))
        result = self.cursor.fetchone()
        return result[0] if result else None

    def add_borrowed_book(self, student_name, book_id, borrow_date, return_date, fine):
        """Adds a new borrowed book record and updates book quantity."""
        self.cursor.execute('''
            INSERT INTO borrowed_books (student_name, book_id, borrow_date, return_date, fine) VALUES (?, ?, ?, ?, ?)
        ''', (student_name, book_id, borrow_date, return_date, fine))
        self.cursor.execute("UPDATE books SET quantity = quantity - 1 WHERE id = ?", (book_id,))
        self.conn.commit()

    def delete_borrowed_book(self, borrowed_book_id, book_id):
        """Deletes a borrowed book record and increments book quantity."""
        self.cursor.execute("DELETE FROM borrowed_books WHERE id = ?", (borrowed_book_id,))
        self.cursor.execute("UPDATE books SET quantity = quantity + 1 WHERE id = ?", (book_id,))
        self.conn.commit()

    def get_book_id_from_borrowed_id(self, borrowed_book_id):
        """Retrieves the book_id associated with a borrowed book record."""
        self.cursor.execute("SELECT book_id FROM borrowed_books WHERE id = ?", (borrowed_book_id,))
        result = self.cursor.fetchone()
        return result[0] if result else None

class LibraryApp:
    """Main Tkinter application for the library management system."""
    def __init__(self, root):
        self.db = Database()
        self.root = root
        self.root.title("Library Management System")
        self.style = Style(theme="flatly")
        self.create_widgets()
        self.refresh_all()

    def create_widgets(self):
        """Configures the main window and its components."""
        # Top frame for search and record count
        top_frame = ttk.Frame(self.root, padding=10)
        top_frame.pack(fill='x')
        
        # Search bar
        search_label = ttk.Label(top_frame, text="Search:")
        search_label.pack(side='left', padx=(0, 5))
        self.search_entry = ttk.Entry(top_frame)
        self.search_entry.pack(side='left', fill='x', expand=True)
        self.search_entry.bind("<KeyRelease>", self.update_table)
        
        # Record count label
        self.record_count_label = ttk.Label(top_frame, text="Records: 0")
        self.record_count_label.pack(side='right', padx=(5, 0))

        # Part 3: Treeview Table
        columns = ("ID", "Student", "Book", "Borrow Date", "Return Date", "Fine")
        self.tree = ttk.Treeview(self.root, columns=columns, show="headings")
        self.tree.pack(fill='both', expand=True, padx=10, pady=(0, 10))

        # Set up columns
        self.tree.column("ID", width=40, anchor='center')
        self.tree.heading("ID", text="ID")
        self.tree.column("Student", width=150, anchor='w')
        self.tree.heading("Student", text="Student")
        self.tree.column("Book", width=150, anchor='w')
        self.tree.heading("Book", text="Book")
        self.tree.column("Borrow Date", width=100, anchor='center')
        self.tree.heading("Borrow Date", text="Borrow Date")
        self.tree.column("Return Date", width=100, anchor='center')
        self.tree.heading("Return Date", text="Return Date")
        self.tree.column("Fine", width=60, anchor='e')
        self.tree.heading("Fine", text="Fine")
        
        # Treeview styling for header and rows
        self.style.configure("Treeview.Heading", font=("TkDefaultFont", 10, "bold"), background="#007bff", foreground="white")
        self.tree.tag_configure('overdue', background='red', foreground='white')
        self.tree.tag_configure('default', background='white', foreground='black')

        # Vertical scrollbar
        scrollbar = ttk.Scrollbar(self.root, orient="vertical", command=self.tree.yview)
        self.tree.configure(yscrollcommand=scrollbar.set)
        scrollbar.pack(side='right', fill='y', pady=(0, 10))

        # Part 2: Borrow Book Form
        form_frame = ttk.LabelFrame(self.root, text="Borrow Book", padding=10)
        form_frame.pack(fill='x', padx=10, pady=10)
        
        # Student Name
        ttk.Label(form_frame, text="Student Name:").grid(row=0, column=0, sticky='w', pady=5, padx=5)
        self.student_name_entry = ttk.Entry(form_frame)
        self.student_name_entry.grid(row=0, column=1, sticky='we', pady=5, padx=5)
        
        # Book Selection
        ttk.Label(form_frame, text="Book:").grid(row=1, column=0, sticky='w', pady=5, padx=5)
        self.book_combo = ttk.Combobox(form_frame, state="readonly")
        self.book_combo.grid(row=1, column=1, sticky='we', pady=5, padx=5)
        
        # Borrow Date
        ttk.Label(form_frame, text="Borrow Date:").grid(row=2, column=0, sticky='w', pady=5, padx=5)
        self.borrow_date_entry = DateEntry(form_frame, date_pattern='yyyy-mm-dd')
        self.borrow_date_entry.grid(row=2, column=1, sticky='we', pady=5, padx=5)
        
        # Return Date
        ttk.Label(form_frame, text="Return Date:").grid(row=3, column=0, sticky='w', pady=5, padx=5)
        self.return_date_entry = DateEntry(form_frame, date_pattern='yyyy-mm-dd')
        self.return_date_entry.grid(row=3, column=1, sticky='we', pady=5, padx=5)

        # Action Buttons
        button_frame = ttk.Frame(form_frame)
        button_frame.grid(row=4, column=0, columnspan=2, pady=10)
        
        add_btn = ttk.Button(button_frame, text="Add", command=self.add_record, bootstyle="success")
        add_btn.pack(side='left', padx=5)
        
        delete_btn = ttk.Button(button_frame, text="Delete", command=self.delete_record, bootstyle="danger")
        delete_btn.pack(side='left', padx=5)
        
        clear_btn = ttk.Button(button_frame, text="Clear", command=self.clear_form, bootstyle="secondary")
        clear_btn.pack(side='left', padx=5)

        # Allow columns to expand
        form_frame.columnconfigure(1, weight=1)

    def refresh_all(self):
        """Refreshes the table and combobox."""
        self.update_table()
        self.populate_book_combo()

    def populate_book_combo(self):
        """Populates the combobox with available books."""
        books = self.db.get_available_books()
        book_titles = [book[1] for book in books]
        self.book_combo['values'] = book_titles
        if book_titles:
            self.book_combo.set(book_titles[0])
        else:
            self.book_combo.set("")

    def calculate_fine(self, return_date_str):
        """Calculates fine based on overdue days."""
        today = datetime.now().date()
        try:
            return_date = datetime.strptime(return_date_str, '%Y-%m-%d').date()
            if return_date < today:
                overdue_days = (today - return_date).days
                return overdue_days * 5
        except ValueError:
            pass  # Invalid date format
        return 0

    def add_record(self):
        """Adds a new borrowed book record to the database."""
        student_name = self.student_name_entry.get().strip()
        book_title = self.book_combo.get()
        borrow_date_str = self.borrow_date_entry.entry.get()
        return_date_str = self.return_date_entry.entry.get()

        if not student_name or not book_title:
            messagebox.showerror("Error", "Student Name and Book are required.")
            return

        # Date validation
        try:
            borrow_date = datetime.strptime(borrow_date_str, '%Y-%m-%d').date()
            return_date = datetime.strptime(return_date_str, '%Y-%m-%d').date()
            if borrow_date > datetime.now().date():
                messagebox.showerror("Error", "Borrow Date cannot be in the future.")
                return
            if return_date < borrow_date:
                messagebox.showerror("Error", "Return Date must not be before Borrow Date.")
                return
        except ValueError:
            messagebox.showerror("Error", "Invalid date format. Use YYYY-MM-DD.")
            return

        book_id = self.db.get_book_id_by_title(book_title)
        fine = self.calculate_fine(return_date_str)
        
        self.db.add_borrowed_book(student_name, book_id, borrow_date_str, return_date_str, fine)
        self.refresh_all()
        self.clear_form()
        messagebox.showinfo("Success", "Record added successfully.")

    def delete_record(self):
        """Deletes the selected record from the Treeview and database."""
        selected_item = self.tree.focus()
        if not selected_item:
            messagebox.showerror("Error", "No record selected.")
            return

        record_id = self.tree.item(selected_item)['values'][0]
        book_id = self.db.get_book_id_from_borrowed_id(record_id)

        if messagebox.askyesno("Confirmation", "Are you sure you want to delete this record?"):
            self.db.delete_borrowed_book(record_id, book_id)
            self.refresh_all()
            messagebox.showinfo("Success", "Record deleted.")

    def clear_form(self):
        """Clears the form entries."""
        self.student_name_entry.delete(0, 'end')
        self.borrow_date_entry.set_date(datetime.now().date())
        self.return_date_entry.set_date(datetime.now().date())
        self.populate_book_combo()

    def update_table(self, event=None):
        """Populates the Treeview with records, filtered by search term."""
        for item in self.tree.get_children():
            self.tree.delete(item)
        
        search_term = self.search_entry.get()
        records = self.db.get_borrowed_books_view(search_term)
        
        for record in records:
            fine = record[5]
            tags = 'overdue' if fine > 0 else 'default'
            self.tree.insert("", "end", values=record, tags=(tags,))
        
        self.record_count_label.config(text=f"Records: {len(records)}")

if __name__ == "__main__":
    root = ttk.Window(themename="flatly")
    app = LibraryApp(root)
    root.mainloop()

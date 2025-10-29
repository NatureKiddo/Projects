// JSON data defined directly in the script
const todos = [
    {
        "UserId": 1,
        "id": 1,
        "title": "Complete my Projects",
        "completed": false
    },
    {
        "UserId": 1,
        "id": 2,
        "title": "Attend team meeting",
        "completed": true
    },
    {
        "UserId": 1,
        "id": 3,
        "title": "Submit assignment",
        "completed": false
    },
    {
        "UserId": 1,
        "id": 4,
        "title": "Prepare for presentation",
        "completed": true
    },
    {
        "UserId": 1,
        "id": 5,
        "title": "Review code changes",
        "completed": false
    },
    {
        "UserId": 1,
        "id": 6,
        "title": "Plan next sprint",
        "completed": false
    },
    {
        "UserId": 1,
        "id": 7,
        "title": "Update documentation",
        "completed": true
    }
];

// DOM elements
const todoList = document.getElementById('todo-list');
const todoForm = document.getElementById('todo-form');
const todoInput = document.getElementById('todo-input');
const filterAllButton = document.getElementById('filter-all');
const filterCompletedButton = document.getElementById('filter-completed');
const filterNotCompletedButton = document.getElementById('filter-not-completed');

// Example: Logged-in user's ID
const currentUserId = 1;

// Render the to-do list
function renderTodos(filteredTodos) {
    const userTodos = filteredTodos.filter(todo => todo.UserId === currentUserId);
    todoList.innerHTML = '';
    userTodos.forEach(todo => {
        const li = document.createElement('li');
        li.dataset.id = todo.id;

        const checkbox = document.createElement('input');
        checkbox.type = 'checkbox';
        checkbox.checked = todo.completed;
        checkbox.addEventListener('change', () => toggleCompletion(todo.id));

        const span = document.createElement('span');
        span.textContent = todo.title;
        if (todo.completed) {
            span.classList.add('completed');
        }

        const deleteButton = document.createElement('button');
        deleteButton.textContent = 'Delete';
        deleteButton.addEventListener('click', () => deleteTask(todo.id));

        li.appendChild(checkbox);
        li.appendChild(span);
        li.appendChild(deleteButton);
        todoList.appendChild(li);
    });
}

// Function to toggle task completion
function toggleCompletion(id) {
    const todo = todos.find(todo => todo.id === id);
    if (todo) {
        todo.completed = !todo.completed;
        renderTodos(todos);
    }
}

// Function to delete a task
function deleteTask(id) {
    const index = todos.findIndex(todo => todo.id === id);
    if (index !== -1) {
        todos.splice(index, 1);
        renderTodos(todos);
    }
}

// Add event listener to the form for adding new tasks
todoForm.addEventListener('submit', (e) => {
    e.preventDefault();
    const newTask = {
        UserId: currentUserId,
        id: todos.length + 1,
        title: todoInput.value.trim(),
        completed: false
    };
    if (newTask.title) {
        todos.push(newTask);
        renderTodos(todos);
        todoInput.value = ''; // Clear the input field
    }
});

// Filter buttons functionality
filterAllButton.addEventListener('click', () => renderTodos(todos));

filterCompletedButton.addEventListener('click', () => {
    const completedTodos = todos.filter(todo => todo.completed);
    renderTodos(completedTodos);
});

filterNotCompletedButton.addEventListener('click', () => {
    const notCompletedTodos = todos.filter(todo => !todo.completed);
    renderTodos(notCompletedTodos);
});

// Render the initial to-do list
document.addEventListener('DOMContentLoaded', () => renderTodos(todos));
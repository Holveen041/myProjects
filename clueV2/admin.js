// DOM элементы
const passwordPrompt = document.getElementById('passwordPrompt');
const adminPanel = document.getElementById('adminPanel');
const loginPassword = document.getElementById('loginPassword');
const loginBtn = document.getElementById('loginBtn');
const loginError = document.getElementById('loginError');
const newPassInput = document.getElementById('newPassInput');
const newHintInput = document.getElementById('newHintInput');
const addBtn = document.getElementById('addBtn');
const removeBtn = document.getElementById('removeBtn');

let adminAuthenticated = false;

// Проверка пароля при входе
async function checkAdminPassword() {
    const password = loginPassword.value.trim();
    
    if (password === "admin123") {
        adminAuthenticated = true;
        passwordPrompt.classList.add('hidden');
        adminPanel.classList.remove('hidden');
    } else {
        loginError.textContent = "Неверный пароль админа";
        loginPassword.value = '';
    }
}

// Добавление пароля
async function addPassword() {
    if (!adminAuthenticated) return;
    
    const newPass = newPassInput.value.trim();
    const newHint = newHintInput.value.trim();
    
    const response = await fetch('/api/add', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ adminPass: "admin123", newPass, newHint })
    });
    
    const result = await response.json();
    
    if (result.error) {
        alert(result.error);
        addBtn.classList.add('error');
        setTimeout(() => addBtn.classList.remove('error'), 500);
    } else {
        alert('Пароль добавлен!');
        addBtn.classList.add('success');
        setTimeout(() => addBtn.classList.remove('success'), 500);
        newPassInput.value = '';
        newHintInput.value = '';
    }
}

// Удаление пароля
async function removePassword() {
    if (!adminAuthenticated) return;
    
    const passToRemove = newPassInput.value.trim();
    
    const response = await fetch('/api/remove', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ adminPass: "admin123", passToRemove })
    });
    
    const result = await response.json();
    
    if (result.error) {
        alert(result.error);
        removeBtn.classList.add('error');
        setTimeout(() => removeBtn.classList.remove('error'), 500);
    } else {
        alert('Пароль удален!');
        removeBtn.classList.add('success');
        setTimeout(() => removeBtn.classList.remove('success'), 500);
        newPassInput.value = '';
        newHintInput.value = '';
    }
}

// Обработчики событий
document.addEventListener('DOMContentLoaded', () => {
    loginBtn.addEventListener('click', checkAdminPassword);
    loginPassword.addEventListener('keypress', (e) => {
        if (e.key === 'Enter') checkAdminPassword();
    });
    addBtn.addEventListener('click', addPassword);
    removeBtn.addEventListener('click', removePassword);
});
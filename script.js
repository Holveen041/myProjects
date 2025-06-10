// DOM элементы
const codeInput = document.getElementById('codeInput');
const checkBtn = document.getElementById('checkBtn');
const hintContainer = document.getElementById('hintContainer');
const hintText = document.getElementById('hintText');
const adminPassInput = document.getElementById('adminPassInput');
const newPassInput = document.getElementById('newPassInput');
const newHintInput = document.getElementById('newHintInput');
const addBtn = document.getElementById('addBtn');
const removeBtn = document.getElementById('removeBtn');

// Проверка пароля
async function checkPassword() {
    const response = await fetch('/api/passwords');
    const passwordsDB = await response.json();
    const enteredCode = codeInput.value.trim();
    
    if (passwordsDB[enteredCode]) {
        checkBtn.classList.add('success');
        setTimeout(() => {
            checkBtn.classList.remove('success');
            showHint(passwordsDB[enteredCode]);
            setTimeout(hideHint, 5000);
        }, 500);
    } else {
        checkBtn.classList.add('error');
        setTimeout(() => {
            checkBtn.classList.remove('error');
            codeInput.value = '';
            codeInput.focus();
        }, 500);
        hideHint();
    }
}

// Добавление пароля
async function addPassword() {
    const adminPass = adminPassInput.value.trim();
    const newPass = newPassInput.value.trim();
    const newHint = newHintInput.value.trim();
    
    const response = await fetch('/api/add', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ adminPass, newPass, newHint })
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
        clearAdminInputs();
    }
}

// Удаление пароля
async function removePassword() {
    const adminPass = adminPassInput.value.trim();
    const passToRemove = newPassInput.value.trim();
    
    const response = await fetch('/api/remove', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ adminPass, passToRemove })
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
        clearAdminInputs();
    }
}

// Вспомогательные функции
function showHint(text) {
    hintText.textContent = text;
    hintContainer.classList.add('hint-visible');
}

function hideHint() {
    hintContainer.classList.remove('hint-visible');
}

function clearAdminInputs() {
    adminPassInput.value = '';
    newPassInput.value = '';
    newHintInput.value = '';
}

// Обработчики событий
document.addEventListener('DOMContentLoaded', () => {
    checkBtn.addEventListener('click', checkPassword);
    codeInput.addEventListener('keypress', (e) => e.key === 'Enter' && checkPassword());
    addBtn.addEventListener('click', addPassword);
    removeBtn.addEventListener('click', removePassword);
});
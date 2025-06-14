// DOM элементы
const codeInput = document.getElementById('codeInput');
const checkBtn = document.getElementById('checkBtn');
const hintContainer = document.getElementById('hintContainer');
const hintText = document.getElementById('hintText');

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

// Вспомогательные функции
function showHint(text) {
    hintText.textContent = text;
    hintContainer.classList.add('hint-visible');
}

function hideHint() {
    hintContainer.classList.remove('hint-visible');
}

// Обработчики событий
document.addEventListener('DOMContentLoaded', () => {
    checkBtn.addEventListener('click', checkPassword);
    codeInput.addEventListener('keypress', (e) => e.key === 'Enter' && checkPassword());
});
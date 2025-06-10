const express = require('express');
const fs = require('fs');
const path = require('path');
const bodyParser = require('body-parser');
const cors = require('cors');
const rateLimit = require('express-rate-limit');

const app = express();
const PORT = 3000;
const DB_FILE = path.join(__dirname, 'passwords.db.json');

// Middleware
app.use(cors());
app.use(bodyParser.json({ limit: '10kb' }));
app.use(express.static(__dirname));

// Защита от DDoS
const limiter = rateLimit({
    windowMs: 15 * 60 * 1000,
    max: 100
});
app.use(limiter);

// Загрузка базы данных
function loadDB() {
    try {
        return JSON.parse(fs.readFileSync(DB_FILE, 'utf8'));
    } catch (e) {
        return {"12345": "Пример подсказки", "qwerty": "Еще пример"};
    }
}

// Сохранение базы данных
function saveDB(data) {
    fs.writeFileSync(DB_FILE, JSON.stringify(data, null, 2));
    return true;
}

// API Routes
app.get('/api/passwords', (req, res) => {
    res.json(loadDB());
});

app.post('/api/add', (req, res) => {
    const { adminPass, newPass, newHint } = req.body;
    const db = loadDB();
    
    if (adminPass !== "admin123") {
        return res.status(403).json({ error: "Неверный пароль админа" });
    }
    
    db[newPass] = newHint;
    saveDB(db);
    res.json({ success: true });
});

app.post('/api/remove', (req, res) => {
    const { adminPass, passToRemove } = req.body;
    const db = loadDB();
    
    if (adminPass !== "admin123") {
        return res.status(403).json({ error: "Неверный пароль админа" });
    }
    
    if (!db[passToRemove]) {
        return res.status(404).json({ error: "Пароль не найден" });
    }
    
    delete db[passToRemove];
    saveDB(db);
    res.json({ success: true });
});

// Запуск сервера
app.listen(PORT, '0.0.0.0', () => {
    console.log(`Сервер запущен:
    - Локально: http://localhost:${PORT}
    - В сети: http://ваш-локальный-ip:${PORT}`);
    
    // Автоматическое определение локального IP (для удобства)
    const os = require('os');
    const interfaces = os.networkInterfaces();
    for (let name in interfaces) {
        for (let net of interfaces[name]) {
            if (net.family === 'IPv4' && !net.internal) {
                console.log(`    - Локальный адрес: http://${net.address}:${PORT}`);
            }
        }
    }
});
const express = require("express");
const fs=require("fs")
const app = express();

function rsplit(str, separator, maxsplit = 1) {
    const parts = str.split(separator);
    if (maxsplit >= parts.length) return parts;
    return [
        parts.slice(0, -maxsplit).join(separator),
        ...parts.slice(-maxsplit)
    ];
}


app.use(express.static(__dirname));


app.get("/get", (req, res) => {
    const fileContent = fs.readFileSync("places.txt", "utf8");
    let a = fileContent.split(")");
    
    for (let i = 0; i < a.length - 1; i++) {
        const parts = rsplit(a[i+1],"\n",1);
        a[i] += ")" + parts[0];
        a[i+1] = parts.slice(1).join("\n");
    }
    
    // Получаем значение из query параметра вместо document.getElementById
    const searchValue = req.query.ttt;
    if (!searchValue) {
        return res.status(400).send("Параметр ttt не указан");
    }
    
    for (let i = 0; i < a.length-1; i++) {
        if (a[i+1].startsWith(searchValue)) {
            return res.send(a[i]);
        }
    }
});


app.get("/", (req, res) => {
    res.sendFile("index.html");
});



app.listen(3000,"0.0.0.0", () => {
    console.log("Сервер запущен на http://localhost:3000");
});

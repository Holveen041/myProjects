function sendRequest(endpoint) {
    const inputValue = document.getElementById("ttt").value;
    if (!inputValue) {
        alert("Пожалуйста, введите значение в поле");
        return;
    }
    
    fetch(`${endpoint}?ttt=${encodeURIComponent(inputValue)}`)
        .then(response => {
            if (!response.ok) {
                throw new Error(`Ошибка HTTP: ${response.status}`);
            }
            return response.text();
        })
        .then(data => document.getElementById("response").innerText = data)
        .catch(error => {
            console.error("Ошибка:", error);
            document.getElementById("response").innerText = "Произошла ошибка: " + error.message;
        });
}
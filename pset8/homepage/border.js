// Get all sliders and theirs value boxes:
let topLeft = document.querySelector("#top-left");
let topLeftValueBox = document.querySelector("#top-left-val");

let topRight = document.querySelector("#top-right");
let topRightValueBox = document.querySelector("#top-right-val");

let bottomRight = document.querySelector("#bottom-right");
let bottomRightValueBox = document.querySelector("#bottom-right-val");

let bottomLeft = document.querySelector("#bottom-left");
let bottomLeftValueBox = document.querySelector("#bottom-left-val");

let widthSlider = document.querySelector("#width");
let widthSliderValueBox = document.querySelector("#width-val");

// Ger preview box
let codeBlockBorderBox = document.querySelector(".code-block-border-box");
let codeBlockBorder = document.querySelector("#code-block-border")
let codeBlockRadius = document.querySelector("#code-block-radius");

// Attach event for each slider
topLeft.addEventListener("change", function(){
    topLeftValueBox.textContent = topLeft.value;
    codeBlockBorderBox.style.borderTopLeftRadius = topLeft.value + "px";
    updateCodeRadius();
});

topRight.addEventListener("change", function(){
    topRightValueBox.textContent = topRight.value;
    codeBlockBorderBox.style.borderTopRightRadius = topRight.value + "px";
    updateCodeRadius();
});

bottomRight.addEventListener("change", function() {
    bottomRightValueBox.textContent = bottomRight.value;
    codeBlockBorderBox.style.borderBottomRightRadius = bottomRight.value + "px";
    updateCodeRadius();
});

bottomLeft.addEventListener("change", function() {
    bottomLeftValueBox.textContent = bottomLeft.value;
    codeBlockBorderBox.style.borderBottomLeftRadius = bottomLeft.value + "px";
    updateCodeRadius();
});

widthSlider.addEventListener("change", function() {
    widthSliderValueBox.textContent = widthSlider.value;
    codeBlockBorderBox.style.borderWidth = widthSlider.value + "px";
    updateCodeBorder();
});

let allBoxes = document.querySelectorAll(".box");
let re = /\w+/;
for (let i = 0; i < allBoxes.length; i++){
    allBoxes[i].addEventListener("click", function(){
        style = this.id.match(re)[0];
        codeBlockBorderBox.style.borderStyle = style;
        updateCodeBorder();
    });
}

// Color picker
let colorPicker = document.querySelector("#color-picker");
colorPicker.addEventListener("change", function() {
    codeBlockBorderBox.style.borderColor = colorPicker.value;
    updateCodeBorder();
});

// Function to update preview box code (border-radius):
function updateCodeRadius() {
    let prefix = "border-radius: ";
    prefix += topLeft.value + "px ";
    prefix += topRight.value + "px ";
    prefix += bottomRight.value + "px ";
    prefix += bottomLeft.value + "px;";
    codeBlockRadius.textContent = prefix;
}

// Function to update preview box code (border):
function updateCodeBorder() {
    let prefix = "border: ";
    let width = widthSlider.value + "px";
    let style = window.getComputedStyle(codeBlockBorderBox).borderStyle;
    let color = window.getComputedStyle(codeBlockBorderBox).borderColor;
    codeBlockBorder.textContent = prefix + width + " " + style + " " + color + ";";
}
// Color range
let colorRange = document.querySelector("#color-range");
let colorsAll = document.querySelectorAll(".color");
let colorLeft = document.querySelector("#color-left");
let colorMiddle = document.querySelector("#color-middle");
let colorRight = document.querySelector("#color-right");
let checkboxMiddle = document.querySelector("#middle-check");
let middleTransitionLabel = document.querySelector("#label-transition");
let middleTransition = document.querySelector("#middle-transition");

// Color picker events
for (let i = 0; i < colorsAll.length; i++) {
    colorsAll[i].addEventListener("change", function() {
        updateColorRange();
        updateGradientPreview();
        updateCodeBlock();
    });
}

// Checkbox event
checkboxMiddle.addEventListener("click", function() {
    updateColorRange();
    updateGradientPreview();
    updateCodeBlock();
});

// Transition change event
middleTransition.addEventListener("change", function() {
    updateColorRange();
    updateGradientPreview();
    updateCodeBlock();
});

// Update color-range function
function updateColorRange() {
    let updateStyle = "linear-gradient(to right, "
    if (checkboxMiddle.checked) {
        
        // Show hidden elements
        colorMiddle.hidden = false;
        middleTransition.hidden = false;
        
        // Update text of transition label
        middleTransitionLabel.textContent = "Middle transition = " + middleTransition.value;
        
        // Prepare string
        updateStyle += colorLeft.value + " 0%, ";
        updateStyle += colorMiddle.value + " " + middleTransition.value +"%, ";
        updateStyle += colorRight.value + " 100%)";
        
        // Update style using prepared string
        colorRange.style.backgroundImage = updateStyle;
    } else {

        // Hide elements
        colorMiddle.hidden = true;
        middleTransition.hidden = true;

        // Update text of transition label
        middleTransitionLabel.textContent = "Middle transition = false";
        
        // Prepare string
        updateStyle += colorLeft.value + " 0%, ";
        updateStyle += colorRight.value + " 100%)";

        // Update style using prepared string
        colorRange.style.backgroundImage = updateStyle;
    }
}

// Gradient preview box
let gradientPreview = document.querySelector("#gradient-box");
let testString = "linear-gradient(rgb(206, 89, 55) 0%, rgb(28, 110, 164) 16%, rgb(197, 146, 55) 100%)"
let testString2 = "radial-gradient(at center center, rgb(206, 89, 55) 0%, rgb(28, 110, 164) 16%, rgb(197, 146, 55) 100%)"
let testString3 = "linear-gradient(135deg, rgb(206, 89, 55) 0%, rgb(28, 110, 164) 16%, rgb(197, 146, 55) 100%)"
let testString4 = "linear-gradient(45deg, rgb(206, 89, 55) 0%, rgb(28, 110, 164) 16%, rgb(197, 146, 55) 100%)"
let re = /\w+-\w+\(([^rgb](\w+\s?,?)+)?/;

// Gradient styles to choose from
let leftToRight = document.querySelector("#left-to-right");
let topToBottom = document.querySelector("#top-to-bottom");
let diagonalDescending = document.querySelector("#diagonal-descending");
let diagonalAscending = document.querySelector("#diagonal-ascending");
let radial = document.querySelector("#radial");
let allStyleBoxes = document.querySelectorAll(".gradient-style-box");

// Add event handlers to eachbox which represents style
for (let i = 0; i < allStyleBoxes.length; i++){
    allStyleBoxes[i].addEventListener("click", function(){
        // Make sure to remove class
        for (let j = 0; j < allStyleBoxes.length; j++){
            allStyleBoxes[j].classList.remove("gradient-style-select");
        }
        // Add it to element that is selected
        this.classList.add("gradient-style-select");
        
        // Change gradient box style and don't change gradient range elemnt
        gradientPreview.style.backgroundImage = window.getComputedStyle(this).backgroundImage;
        updateGradientPreview();
        updateCodeBlock();
    });
}

// Update gradient preview function
function updateGradientPreview() {
    let updateStyle = window.getComputedStyle(gradientPreview).backgroundImage.match(re)[0]
    if (checkboxMiddle.checked) {
        updateStyle += colorLeft.value + " 0%, ";
        updateStyle += colorMiddle.value + " " + middleTransition.value + "%, ";
        updateStyle += colorRight.value + " 100%)";
        gradientPreview.style.backgroundImage = updateStyle;
    } else {
        updateStyle += colorLeft.value + " 0%, ";
        updateStyle += colorRight.value + " 100%)";
        gradientPreview.style.backgroundImage = updateStyle;
    }
}

// Update code function
let  codeBlock = document.querySelector("#code-block");

function updateCodeBlock() {
    let currentGradientStyle = getComputedStyle(gradientPreview).backgroundImage;
    codeBlock.textContent = currentGradientStyle;
}

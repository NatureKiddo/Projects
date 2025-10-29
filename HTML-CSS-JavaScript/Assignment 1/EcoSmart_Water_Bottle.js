// Get references to elements
const buyNowBtn = document.getElementById("buyNowBtn");
const modal = document.getElementById("modal");
const closeBtn = document.querySelector(".close-btn");
const colorSelect = document.getElementById("color");
const engravingCheckboxes = document.querySelectorAll(".engraving"); // Select all engraving checkboxes
const totalPriceSpan = document.getElementById("totalPrice");
const contactForm = document.getElementById("contactForm");
const checkoutBtn = document.getElementById("checkoutBtn");

// Function to calculate and update the total price
function updateTotalPrice() {
    const colorPrice = parseFloat(colorSelect.value); // Get the selected color price
    let engravingPrice = 0.00;

    // Loop through all engraving checkboxes and add their values if checked
    engravingCheckboxes.forEach((checkbox) => {
        if (checkbox.checked) {
            engravingPrice += parseFloat(checkbox.value);
        }
    });

    const totalPrice = colorPrice + engravingPrice; // Calculate total price
    totalPriceSpan.textContent = `R${totalPrice.toFixed(2)}`; // Update the total price in the modal with two decimal places
    return totalPrice; // Return the total price for further use
}

// Show the modal when "Buy Now" button is clicked
buyNowBtn.addEventListener("click", function () {
    console.log("Buy Now button clicked");
    updateTotalPrice(); // Update the total price when the modal is shown
    modal.classList.remove("hidden"); // Show the modal
    modal.setAttribute("aria-hidden", "false"); // Update accessibility
});

// Hide the modal when the close button is clicked
closeBtn.addEventListener("click", function () {
    modal.classList.add("hidden"); // Hide the modal
    modal.setAttribute("aria-hidden", "true"); // Update accessibility
});

// Optional: Hide the modal when clicking outside the modal content
window.addEventListener("click", function (event) {
    if (event.target === modal) {
        modal.classList.add("hidden");
        modal.setAttribute("aria-hidden", "true"); // Update accessibility
    }
});

// Update the total price dynamically when the color is changed
colorSelect.addEventListener("change", updateTotalPrice);

// Update the total price dynamically when any engraving checkbox is toggled
engravingCheckboxes.forEach((checkbox) => {
    checkbox.addEventListener("change", updateTotalPrice);
});

// Handle the checkout button click
checkoutBtn.addEventListener("click", function () {
    const finalPrice = updateTotalPrice(); // Get the final total price
    const messageContainer = document.createElement("p"); // Create a new paragraph element
    messageContainer.textContent = `Total Price: R${finalPrice.toFixed(2)}`; // Display the final price with two decimal places
    messageContainer.style.color = "black"; // Add some styling to the message
    messageContainer.style.marginTop = "10px"; // Add spacing above the message

    // Append the message below the "Buy Now" button
    const homeSection = document.getElementById("home");
    homeSection.appendChild(messageContainer);

    // Hide the modal after checkout
    modal.classList.add("hidden");
    modal.setAttribute("aria-hidden", "true");
});

// Validate the contact form to ensure Terms & Conditions checkbox is checked
contactForm.addEventListener("submit", function (event) {
    if (!document.getElementById("terms").checked) {
        alert("You must agree to the terms and conditions."); // Show alert if checkbox is not checked
        event.preventDefault(); // Prevent form submission
    }
    
});
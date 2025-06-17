const loginButton = document.getElementById('loginButton');
const loginForm = document.getElementById('loginForm');

loginButton.addEventListener('click', () => {
  loginForm.style.display = loginForm.style.display === 'block' ? 'none' : 'block';
});

// Fermer le dropdown si on clique en dehors
window.addEventListener('click', (e) => {
  if (!loginButton.contains(e.target) && !loginForm.contains(e.target)) {
    loginForm.style.display = 'none';
  }
});

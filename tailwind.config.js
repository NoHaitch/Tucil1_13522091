/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    "./src/templates/**/*.html",
    "./src/static/src/**/*.js"
  ],
  theme: {
    extend: {
      colors: {
        transparent: 'transparent',
        current: 'currentColor',
        'midnight': '#01012b',
        'secondary': '#ff2a6a',
        'font-1': '#05d9e8',
      },
    },
  },
  plugins: [],
}
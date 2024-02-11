<!-- Back to Top Link-->
<a name="readme-top"></a>


<br />
<div align="center">
  <h1 align="center">Tucil 1 - Stima</h1>

  <p align="center">
    <h3> Cyberpunk 2077 Breach Protocol Solver</h3>
    <h4> Using Brute Force Algorithm</h4>
    <br/>
    <a href="#">Report Bug</a>
    ·
    <a href="#">Request Feature</a>
<br>
<br>

[![MIT License][license-shield]][license-url]

  </p>
</div>

<!-- CONTRIBUTOR -->
<div align="center" id="contributor">
  <strong>
    <h3>Made By:</h3>
    <h3>Raden Francisco Trianto Bratadiningrat</h3>
    <h3>NIM: 13522091</h3>
  </strong>
  <br>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
        <li><a href="#instruction">Instruction</a></li>
        <li>
        <details>
          <summary><a href="#features">Features</a></summary>
          <ol>
            <li><a href="#1-solving-the-minigame">Solving</a></li>
            <li><a href="#2-read-from-txt-file">Read from File</a></li>
            <li><a href="#3-randomize-matrix-and-sequance-using-cli">Randomize matrix and sequance</a></li>
            <li><a href="#4-save-the-result-as-a-txt-file">Save to file</a></li>
          <ol>
        </details>
        </li>
      </ul>
    </li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#special-thanks">Special Thanks</a></li>
  </ol>
</details>
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ABOUT THE PROJECT -->
## About The Project

This program is made to solve Cyberpunk 2077 Breach Protocol or Hacking minigame with the highest amount of reward and least amount of buffer used. 

The project itself is made for me to learn more about one the most basic type of algorithm design which is brute force. The project is also a task for IF2211 - Strategi Algoritma.

<!-- OPTIONAL LINK OR REFERENCE -->
<!-- <p align="center">
You can explore more on this link ... 
<br>
<a href="https://example.com"> <Strong>THIS LINK</Strong>
</a>
</p> -->

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

Project dependencies  
example :  

* Make 
  ```sh
  # in Linux
  sudo apt install make
  ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/NoHaitch/Tucil1_13522091
   ```
2. Run the program
   ```sh
   make all
   ```


<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- INSTURCTION -->
## Instruction

Using Makefile commands:

`make run` : run the compiled program  
`make build` : build source to binary in bin/main  
`make clean` : clean all file in bin
`make all` : clean build and run the program  


<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- FEATURES -->
## Features

### 1. Solving the minigame

This feature is the base of the program. It solves the minigame through a brute force algorithm, finding the buffer that gives the most amount of reward with the least amount of buffer used.

### 2. Read from .txt file

The program can read txt files with the format of data as follows:
- buffer_size
- matrix_width matrix_height
- matrix
- number_of_sequences
- sequences_1
- sequences_1_reward
- sequences_2
- sequences_2_reward  
...
- sequences_n
- sequences_n_reward

*The file must be inside the 'test' directory.*

Example: test/test.txt

### 3. Randomize matrix and sequance using CLI

The program can generate a random board with random sequance and rewards using some data:
- Amount of Unique Tokens
- Unique Tokens
- Buffer Size
- Matrix Size
- Amount of Sequance
- Max Length of a Sequance

### 4. Save the result as a txt file

The program saves the result of solving the minigame to a file in the test directory.

Example: test/output.txt




<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- CONTRIBUTING -->
## Contributing

If you want to contribute or further develop the program, please fork this repository using the branch feature.  
Pull Request is **permited and warmly welcomed**

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## Licensing

The code in this project is licensed under MIT license.  


<!-- SPECIAL THANKS AND/OR CREDITS -->
## Special Thanks
- [Best-README-Template](https://github.com/othneildrew/Best-README-Template) by othneildrew

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<br>
<h3 align="center"> THANK YOU! </h3>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[issues-url]: https://github.com/NoHaitch/Tucil1_13522091/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/NoHaitch/Tucil1_13522091/blob/main/LICENSE
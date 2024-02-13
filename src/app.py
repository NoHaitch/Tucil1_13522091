from flask import Flask, request, render_template
from werkzeug.utils import secure_filename
import os
from subprocess import Popen, PIPE, STDOUT

UPLOAD_FOLDER = '../test/'
ALLOWED_EXTENSIONS = {'txt'}

app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "GET":
        return render_template("index.html")
    else:
        file_uploaded = False

        if 'file_input' in request.files:
            file = request.files['file_input']

            if file:
                if file.filename.split('.')[-1].lower() not in ALLOWED_EXTENSIONS:
                    return 'Invalid file type!'

                filename = secure_filename(file.filename)
                filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
                file.save(filepath)

                file_uploaded = True

            else:
                bufferSize = request.form["bufferSize"]
                width = request.form["width"]
                height = request.form["height"]
                tokenAmount = request.form["tokenAmount"]
                uniqueToken = request.form["uniqueToken"]
                seqAmount = request.form["seqAmount"]
                maxSeqSize = request.form["maxSeqSize"]
                if(bufferSize and width and height and tokenAmount and uniqueToken and seqAmount and maxSeqSize):
                    pass
                else:
                    return render_template("index.html")
        else:
            bufferSize = request.form["bufferSize"]
            width = request.form["width"]
            height = request.form["height"]
            tokenAmount = request.form["tokenAmount"]
            uniqueToken = request.form["uniqueToken"]
            seqAmount = request.form["seqAmount"]
            maxSeqSize = request.form["maxSeqSize"]
            if(bufferSize and width and height and tokenAmount and uniqueToken and seqAmount and maxSeqSize):
                pass
            else:
                return render_template("index.html")

        if file_uploaded:
            command = ["../bin/main.exe"]

            process = Popen(command, stdin=PIPE, stdout=PIPE)

            input_data = "1 " + filename + " y 3"
            print(f"INPUT DATA: {input_data}")
            input_bytes = input_data.encode()
            process.stdin.write(input_bytes)
            process.stdin.close()

            output, err = process.communicate()
            max_points, buffer, matrixPath, time, width, height, matrix, seq = parse_result("../test/output.txt")

            if(time == -1):
                return render_template("index.html")    
            

            return render_template("result.html", 
                         buffer=buffer, 
                         matrixPath=matrixPath, 
                         time=time, 
                         max_points=max_points, 
                         width=width, 
                         height=height, 
                         matrix=matrix, 
                         seq=seq)
        
        else:
            command = ["../bin/main.exe"]
    
            process = Popen(command, stdin=PIPE, stdout=PIPE)

            input_data = "2 " + tokenAmount + " " + uniqueToken + " " + bufferSize + " " + width + " " + height  + " " + seqAmount + " " + maxSeqSize + " y 3"
            print(f"INPUT DATA: {input_data}")
            input_bytes = input_data.encode()
            process.stdin.write(input_bytes)
            process.stdin.close()

            output, err = process.communicate()
            result = output.decode()
            max_points, buffer, matrixPath, time, width, height, matrix, seq = parse_result("../test/output.txt")

            if(time == -1):
                return render_template("index.html")    
            

            return render_template("result.html", 
                         buffer=buffer, 
                         matrixPath=matrixPath, 
                         time=time, 
                         max_points=max_points, 
                         width=width, 
                         height=height, 
                         matrix=matrix, 
                         seq=seq)

def callSolve(argument):
    # Replace with your specific argument processing logic
    # Here's an example:
    result = argument.upper()  # Make uppercase for demonstration
    return result

def parse_result(filename):
    try:
        with open(filename, "r") as f:
            lines = f.readlines()

        # Parse max points
        try:
            max_points = int(lines[0].strip())
        except ValueError:
            raise ValueError("Invalid format: Invalid max points")
        # Parse buffer size
        try:
            buffer = lines[1]
        except (ValueError, IndexError):
            raise ValueError("Invalid format: Invalid buffer")

        # Parse matrix path
        try:
            matrixPath = []
            time = -1
            for line in lines[2:-1]:
                row, col = map(int, line.split(", "))
                matrixPath.append([row, col])
        except (ValueError, IndexError):
            print("error")

        # Parse time
        try:
            time_str = lines[-1].split(": ")[-1].strip()
            time = int(time_str.split()[0])
        except (ValueError, IndexError):
            print(line)

        with open("../bin/temp", "r") as f:
            lines = f.readlines()
        
        try:
            size = lines[0].split(" ")
            width = int(size[0])
            height = int(size[1])
        except (ValueError, IndexError):
            print(line)

        try:
            matrix = []
            for row in lines[1:height+1]:
                matrix.append(row.split(" ")[:-1])

        except (ValueError, IndexError):
            print(line)

        try:
            seq = []
            for row in lines[height+1:]:
                seq.append(row.split(" ")[:-1])

        except (ValueError, IndexError):
            print(line)

        print(matrixPath)
        return max_points, buffer, matrixPath, time, width, height, matrix, seq

    except FileNotFoundError:
        raise ValueError("File not found")

if __name__ == "__main__":
    app.run(debug=True)


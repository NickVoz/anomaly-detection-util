# **Anomaly Detection Utility**

This program  detects anomalies in a received dataset.
We created this program during one of our programming courses, it utilises various design patters, such as the Command pattern.

The program first learns aobut correlations(using pearson correlation function) between various features in one dataset.
Afterwards, it can receive additional datasets with the same features and detect anomalies from the detected correlation values.
Correlation requirements such as desired threshold can be tweaked using the program's menu.

The programs comes with a command line interface, and is guaranteed to work on various Linux distors.

### Executing the program
  * First, use `git clone` to fetch the repository
  * Use `make` to compile the source code(works on Linux)
  * a main.out file will be created, use `./main.out` to execute it.


Example input(line by line) and output for the program are provided in the SampleInput/input.txt file.

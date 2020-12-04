# RGB_To_Gray-CUDA

## Build
```
$ make 
```

## Parameters (CLI)
| Parameters                  | Type      | Value | Description |
|-----------------------------|:---------:|:------:|-------------|
| -input                      | String    | Required | Path to folder containing the input image |
| -output                     | String    | Required | Path to folder where save the output images |


## Run
Exemple: 
```
$ ./rgb_to_gray -input <input_path> -output <output_path>
```

## Remove executable
```
$ make clean
```
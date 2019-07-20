# LZArchiver
LZ Archiver - simple C++ CLI realization of LZ77 and LZ78 data compression algorithms. Windows tested only.


# Usage

```LZArchiver.exe -c|-x input_file [--output=output_file] [--type=-lz77|-lz78 [--rate=-l|-m|-h](only for lz77)]```

```-c``` - compression mode

```-x``` - extraction mode

```--output=output_file``` - output file flag. By default = **input_name** with **.arch** extension for compression mode, **input_name** without **.arch** extension for extraction mode

```--type=-lz77|-lz78``` - algo selection flag

```--rate=-l|-m|-h``` - compression rate parameter (low, middle, high) for LZ77 only


# Saving format

**.arch** - developed binary file saving format for LZ77/LZ78 algorithms.

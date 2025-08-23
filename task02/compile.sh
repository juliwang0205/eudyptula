#!/bin/bash

# Navigate to the kernel source directory
# cd /path/to/your/kernel/source

# Create a localmodconfig
yes "" | make localmodconfig

# (Optional) Check for errors and exit if any
if [ $? -ne 0 ]; then
  echo "Error during make localmodconfig"
  exit 1
fi

# (Optional) Build the kernel with the new config
# make -j$(nproc) # Example with parallel build, adjust as needed
# make modules_install install # Example with modules and install

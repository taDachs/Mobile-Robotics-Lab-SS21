#!/usr/bin/env python3

import csv
import argparse
import os

import matplotlib.pyplot as plt
import numpy as np

parser = argparse.ArgumentParser(description='Parses a csv dump from sensors.')
parser.add_argument('--input', type=str, help='path to csv file')
parser.add_argument('--output', type=str, help='path to output dir')

args = parser.parse_args()

labels = ['lineSensor_middle', 'encoder_left', 'lineSensor_right', 'battery', 'encoder_right', 'lineSensor_left']

data = {label: [] for label in labels}

with open(args.input) as f:
    reader = csv.reader(f, delimiter=';')

    for row in reader:
        row = row[:-1]

        for key, value in zip(labels, row):
            data[key].append(float(value))

for label in labels:
    fig, ax = plt.subplots(figsize=(10, 10))
    ax.locator_params(axis="y", nbins=20)
    ax.plot(data[label])
    path = os.path.join(args.output, f'{label}.png')
    fig.savefig(path)
    print(f'saved data for {path}')

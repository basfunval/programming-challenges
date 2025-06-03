#!/bin/bash
grep "$(passgen -l 2 -c number)" programming-ideas.txt

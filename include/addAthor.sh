#!/bin/bash

for fichier in *.h
do
      (echo "/* @Author BELLATIF Mamoune & BARKAOUI Meriam */" && cat $fichier) > $fichier.1 && mv $fichier.1 $fichier
done

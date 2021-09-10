#!/usr/bin/env bash
 for file in /data/astro/dragon/S1843/data/*.mid;
     do echo ${file#/data/astro/dragon/S1843/data/}
         xmlFile=${file/%mid/xml}
         name=${file#/data/astro/dragon/S1843/data/}
         echo $xmlFile
         #[ -f $xmlFile ] && /home/sriteja/packages/dragon/analyzer/bin/    mid2root "$file" -v "$xmlFile" -o "${name/%mid/root}" || /home/sriteja/    packages/dragon/analyzer/bin/mid2root "$file" -o "${name/%mid/root}";
         if [ -f $xmlFile ]; then
             /home/sriteja/packages/dragon/analyzer/bin/mid2root "$file" -  v "$xmlFile" -o "${name/%mid/root}"
             echo "Converting... With XML"
         else
             /home/sriteja/packages/dragon/analyzer/bin/mid2root "$file" -  o "${name/%mid/root}"
             echo "No XML file. Converting..."
         fi
 done

find -name build.xml | awk -F"/" '$0 !~ /infuser/ {print "ant clean -f "$0}' | sh
find -name build.xml | awk -F"/" '$0 ~ /infuser/ {print "ant clean -f "$0}' | sh

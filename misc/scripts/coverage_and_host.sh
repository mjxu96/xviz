SOURCE_DIR=$(realpath $( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )/../../)
echo $SOURCE_DIR
lcov -c -b $SOURCE_DIR/include -d $SOURCE_DIR/build -o $SOURCE_DIR/build/coverage.info --include "$SOURCE_DIR/*" --exclude "$SOURCE_DIR/build/*"
genhtml $SOURCE_DIR/build/coverage.info --output-directory $SOURCE_DIR/build/html/
python3 -m http.server -d $SOURCE_DIR/build/html/ -b 0.0.0.0
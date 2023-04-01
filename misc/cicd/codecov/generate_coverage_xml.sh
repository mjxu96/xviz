
TEST_FOLDER=$(realpath ${1:-.})
echo "test path: $TEST_FOLDER"
SOURCE_DIR=$(realpath $( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )/../../../)
echo "source path: $SOURCE_DIR"
lcov -c -d $SOURCE_DIR -o $SOURCE_DIR/coverage.info --include "$SOURCE_DIR/*" --exclude "$SOURCE_DIR/build/*"
lcov_cobertura coverage.info -d -b $TEST_FOLDER -e "tests/*" -o coverage.xml

cd ..
git clone git@git.forge.epita.fr:p/epita-ing-assistants-acu/42sh-2027/epita-ing-assistants-acu-42sh-2027-strasbourg-07.git

cp 42sh/Makefile.am epita-ing-assistants-acu-42sh-2027-strasbourg-07
cp 42sh/configure.ac epita-ing-assistants-acu-42sh-2027-strasbourg-07
cp -r 42sh/src epita-ing-assistants-acu-42sh-2027-strasbourg-07
cp -r 42sh/tests epita-ing-assistants-acu-42sh-2027-strasbourg-07

cd epita-ing-assistants-acu-42sh-2027-strasbourg-07
autoreconf --install
./configure
make

if [ $? -ne 0 ]; then
    echo "SURTOUT PAS DE PUSH"
    exit 1
fi

make check

if [ $? -ne 0 ]; then
    echo "Les tests ne passent pas mais tkt"
fi

git add Makefile.am configure.ac src/*.c src/*.am src/*/*.c src/*/*.h src/*/*.am tests/*
git commit -m "fix: 42sh"
git push
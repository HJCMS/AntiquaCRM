#!/usr/bin/env bash

_lupdate=$HOME/Programme/Qt/5.15.2/gcc_64/bin/lupdate

${_lupdate} -no-obsolete . -ts $(ls i18n/*.ts) -recursive

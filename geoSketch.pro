######################################################################
# Automatically generated by qmake (2.01a) Sun Mar 13 12:46:42 2011
######################################################################

TEMPLATE = subdirs
QT += core gui opengl
SUBDIRS = app common test
app.depends = common
test.depends = common app


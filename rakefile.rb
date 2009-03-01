HERE = File.expand_path(File.dirname(__FILE__)) + '/'

require 'rake'
require 'rake/clean'
require 'rake/testtask'

#Tool and Lib Locations
C_COMPILER = 'gcc'
C_LIBS = ''
UNITY_DIR = 'vendor/unity/src'

#Test File To Be Created
OUT_FILE = 'test_cexceptions'
OUT_EXTENSION = '.out'

#Options
SRC_FILES = "lib/CException.c test/TestException.c test/TestException_Runner.c #{UNITY_DIR}/unity.c"
INC_DIRS = "-Ilib -Itest -I#{UNITY_DIR}"
LIB_DIRS = C_LIBS.empty? ? '' : "-L#{C_LIBS}"
SYMBOLS = '-DTEST'

CLEAN.include("#{HERE}*.out")

task :default => [:clobber, :test]

desc "performs a quick set of unit tests to confirm you're ready to go"
task :test do
  puts "#{C_COMPILER} #{INC_DIRS} #{LIB_DIRS} #{SYMBOLS} #{SRC_FILES} -o #{OUT_FILE}#{OUT_EXTENSION}"
  output = `#{C_COMPILER} #{INC_DIRS} #{LIB_DIRS} #{SYMBOLS} #{SRC_FILES} -o #{OUT_FILE}#{OUT_EXTENSION}`
  puts output

  puts "#{HERE}#{OUT_FILE}#{OUT_EXTENSION}"
  output = `#{HERE}#{OUT_FILE}#{OUT_EXTENSION}`
  puts output
end
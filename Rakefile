HERE = File.expand_path(File.dirname(__FILE__)) + '/'

require 'rake'
require 'rake/clean'
require 'rake/testtask'
require HERE+'vendor/unity/auto/colour_reporter.rb'

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
SYMBOLS = '-DTEST -DCEXCEPTION_USE_CONFIG_FILE'

CLEAN.include("#{HERE}*.out")

task :no_color do
  $colour_output = false
end

desc "performs a quick set of unit tests to confirm you're ready to go"
task :test do
  report "#{C_COMPILER} #{INC_DIRS} #{LIB_DIRS} #{SYMBOLS} #{SRC_FILES} -o #{OUT_FILE}#{OUT_EXTENSION}"
  output = `#{C_COMPILER} #{INC_DIRS} #{LIB_DIRS} #{SYMBOLS} #{SRC_FILES} -o #{OUT_FILE}#{OUT_EXTENSION}`
  report output

  report "#{HERE}#{OUT_FILE}#{OUT_EXTENSION}"
  output = `#{HERE}#{OUT_FILE}#{OUT_EXTENSION}`
  report output
end

task :default => [:clobber, :test]
task :ci => [:no_color, :default]
task :cruise => :ci

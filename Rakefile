# * Encoding: UTF-8
# Copyright © 2009 Evan Phoenix and The Rubyists, LLC
# Distributed under the terms of the MIT license.
# See the LICENSE file which accompanies this software for the full text
begin; require 'rubygems'; rescue LoadError; end

require 'rake'
require 'rake/clean'
require 'rake/gempackagetask'
require 'time'
require 'date'

PROJECT_SPECS = FileList[
  'spec/url_escape.rb',
  'spec/bench.rb'
]

PROJECT_MODULE = 'URLEscape'
PROJECT_README = 'README'

PROJECT_COPYRIGHT_SUMMARY = [
 "# Copyright © 2009 Evan Phoenix and The Rubyists, LLC",
 "# Distributed under the terms of the MIT license.",
 "# See the LICENSE file which accompanies this software for the full text",
]
PROJECT_COPYRIGHT = PROJECT_COPYRIGHT_SUMMARY + [
 "#",
 "# Permission is hereby granted, free of charge, to any person obtaining a copy",
 '# of this software and associated documentation files (the "Software"), to deal',
 "# in the Software without restriction, including without limitation the rights",
 "# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell",
 "# copies of the Software, and to permit persons to whom the Software is",
 "# furnished to do so, subject to the following conditions:",
 "#",
 "# The above copyright notice and this permission notice shall be included in",
 "# all copies or substantial portions of the Software.",
 "#",
 '# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR',
 "# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,",
 "# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE",
 "# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER",
 "# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,",
 "# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN",
 "# THE SOFTWARE."
]

PROJECT_VERSION =
  if version = ENV['PROJECT_VERSION'] || ENV['VERSION']
    version
  else
    URLEscape::VERSION rescue Date.today.strftime("%Y.%m.%d")
  end

# To release the monthly version do:
# $ PROJECT_VERSION=2009.03 rake release

GEMSPEC = Gem::Specification.new{|s|
  s.name         = "url_escape"
  s.description  = "Fast replacement for CGI.escape and Rack::Utils.escape"
  s.authors      = ["Evan Phoenix", "TJ Vanderpoel", "Michael Fellinger", "Trey Dempsey", "Jayson Vaughn"]
  s.summary      = "Fast url_escape library written in C"
  s.email        = "manveru@rubyists.com"
  s.homepage     = "http://github.com/bougyman/seedling"
  s.rubyforge_project = "url-escape"
  s.platform     = Gem::Platform::RUBY
  s.version      = PROJECT_VERSION
  s.files        = %w(RELEASE AUTHORS CHANGELOG MANIFEST README Rakefile) +
                   Dir['ext/*.{c,h,rb}'] +
                   Dir['spec/*.rb'] +
                   Dir['tasks/*.rake']
  s.has_rdoc     = false
  s.extensions   = ['ext/extconf.rb']
  s.require_path = "ext"
}

if RUBY_PLATFORM =~ /mswin/
  # Win specific stuff
elsif RUBY_PLATFORM =~ /java/
  GEMSPEC.platform = 'java'
  GEMSPEC.files += %w[ lib/url_escape.rb ]
  GEMSPEC.require_path = "lib"
  GEMSPEC.extensions = nil
end

Dir.glob('tasks/*.rake'){|f| import(f) }

task :default => [:bacon]

CLEAN.include %w[
  **/.*.sw?
  *.gem
  .config
  **/*~
  **/{data.db,cache.yaml}
  *.yaml
  pkg
  rdoc
  ydoc
  *coverage*
]

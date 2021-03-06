namespace :release do
  task :all => [:release_github, :release_rubyforge]

  desc 'Display instructions to release on github'
  task :github => [:reversion, :gemspec] do
    name, version = GEMSPEC.name, GEMSPEC.version

    puts <<INSTRUCTIONS
First add the relevant files:

git add AUTHORS MANIFEST CHANGELOG #{name}.gemspec lib/#{name}/version.rb

Then commit them, tag the commit, and push:

git commit -m 'Version #{version}'
git tag -a -m '#{version}' '#{version}'
git push

INSTRUCTIONS

  end

  # TODO: Not tested
  desc 'Display instructions to release on rubyforge'
  task :rubyforge => [:reversion, :gemspec, :package] do
    rf_project, name, version = GEMSPEC.rubyforge_project, GEMSPEC.name, GEMSPEC.version.to_s

    puts <<INSTRUCTIONS
To publish to rubyforge do following:

rubyforge login
rubyforge #{/java/i === RUBY_PLATFORM ? "add_file" : "add_release"} #{rf_project} #{name} #{version.dump} pkg/#{name}-#{version}#{/java/i === RUBY_PLATFORM ? "-java" : ""}.gem

After you have done these steps, see:

VERSION=#{version.dump} rake release:rubyforge_archives

INSTRUCTIONS
  end

  desc 'Display instructions to add archives after release:rubyforge'
  task :rubyforge_archives do
    rf_project, name, version = GEMSPEC.rubyforge_project, GEMSPEC.name, GEMSPEC.version.to_s
    puts "Adding archives for distro packagers is:", ""

    Dir["pkg/#{name}-#{version}.{tgz,zip}"].each do |file|
      puts "rubyforge add_file %s %s %p %p" % [rf_project, name, version, file]
    end

    puts
  end
end

require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))
folly_compiler_flags = '-DFOLLY_NO_CONFIG -DFOLLY_MOBILE=1 -DFOLLY_USE_LIBCPP=1 -Wno-comma -Wno-shorten-64-to-32'

Pod::Spec.new do |s|
  s.name         = "rn-wav-to-flac"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = package["homepage"]
  s.license      = package["license"]
  s.authors      = package["author"]

  s.platforms    = { :ios => "10.0" }
  s.source       = { :git => "https://github.com/kleydon/rn-wav-to-flac.git", :tag => "#{s.version}" }

  # Modified section
  # ++++++++++++++++
  # Modified to admit 'c' and 'hpp' files
  s.source_files = "ios/**/*.{h,m,mm}", "cpp/**/*.{h,c,hpp,cpp}"

  # Added to specify header search paths for BUILDING the pod
  # Idea from here: https://github.com/CocoaPods/CocoaPods/issues/5375
  s.pod_target_xcconfig = {
    'HEADER_SEARCH_PATHS' => '$(inherited) ${PODS_TARGET_SRCROOT}/ios/** ${PODS_TARGET_SRCROOT}/cpp/**'
  }

  # Added to expose a heirarchical header structure for the USERS of this pod.
  # Other methods, e.g. s.private_header_files, s.public_header_files etc, 
  # flatten the heirarchy, resulting in naming collisions and confusion.
  s.header_mappings_dir = 'cpp/include'
  # ----------------

  
  s.dependency "React-Core"

  # Don't install the dependencies when we run `pod install` in the old architecture.
  if ENV['RCT_NEW_ARCH_ENABLED'] == '1' then
    s.compiler_flags = folly_compiler_flags + " -DRCT_NEW_ARCH_ENABLED=1"
    s.pod_target_xcconfig    = {
        "HEADER_SEARCH_PATHS" => "\"$(PODS_ROOT)/boost\"",
        "CLANG_CXX_LANGUAGE_STANDARD" => "c++17"
    }

    s.dependency "React-Codegen"
    s.dependency "RCT-Folly"
    s.dependency "RCTRequired"
    s.dependency "RCTTypeSafety"
    s.dependency "ReactCommon/turbomodule/core"
  end
end

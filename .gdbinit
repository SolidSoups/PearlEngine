file build/game_engine

define build
  set confirm off
  if $_isrunning
    kill
  end
  set confirm on
  shell ./compile_build.sh
  if $_shell_exitcode == 0
    run
  end
end


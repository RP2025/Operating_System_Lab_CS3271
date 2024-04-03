#!/bin/bash

# Function to handle internal commands
internal_commands() {
    case $1 in
        cd) 
            # Handle 'cd' command with error checking
            if [[ -z $2 ]]; then
                cd "$HOME" || echo "Error: Unable to change directory"
            else
                cd "$2" || echo "Error: Unable to change directory to $2"
            fi
            ;;
        pwd) 
            pwd ;;
        clear) 
            clear ;;
        exit) 
            exit ;;
        *) 
            return 1 ;;
    esac
    return 0
}

# Function to execute external commands
execute_external() {
    # Execute external command with error handling
    "$@" || echo "Error: Failed to execute $1"
}

# Function to handle command chaining
handle_command_chain() {
    local command="$1"
    local operator="$2"
    
    case $operator in
        "&&")
            $command && return 0 ;;
        "||")
            $command || return 1 ;;
        ";")
            $command ;;
    esac
}

# Main loop
while true; do
    # Show shell prompt
    printf "%s " "$USER@$HOSTNAME:$PWD$ "
    
    # Read command from user
    read -r cmd
    
    # Save command to history
    history -s "$cmd"
    
    # Split commands by operators (;, &&, ||)
    IFS=";&" read -ra commands <<< "$cmd"
    
    # Iterate over commands
    for command in "${commands[@]}"; do
        # Remove leading and trailing whitespaces
        command=$(echo "$command" | xargs)
        
        # Split command into command and arguments
        IFS=" " read -ra parts <<< "$command"
        
        # Check if it's an internal command
        if internal_commands "${parts[0]}" "${parts[@]:1}"; then
            continue
        fi
        
        # Check if command contains environment variables
        eval_command=$(eval echo "$command")
        
        # Execute external command
        handle_command_chain "execute_external $eval_command" "${parts[-1]}"
        
        # Check for command success or failure
        if [[ $? -ne 0 ]]; then
            echo "Command failed: $command"
        fi
    done
done


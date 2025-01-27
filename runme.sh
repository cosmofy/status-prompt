#!/bin/bash

# Change to the directory where the script is located
cd "$(dirname "$0")"

ssh aws-ec2 "./prepare_keys"
scp aws-ec2:encrypted_keys.gpg .

gpg --batch --passphrase-file password.txt --decrypt encrypted_keys.gpg

./build/status_tui encrypted_keys

rm encrypted_keys
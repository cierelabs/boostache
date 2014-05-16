Company {{NAME}}

{{#YEARS}}
{{#DIVISIONS}}
Division: {{NAME}} Budget for year {{YEAR}}: {{BUDGET}}
Employees:
{{#EMPLOYEES}} {{NAME}} salary {{SALARY}}
{{/EMPLOYEES}}
{{^EMPLOYEES}}
Has no employees
{{/EMPLOYEES}}
{{/DIVISIONS}}
{{/YEARS}}



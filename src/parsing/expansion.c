#include "minishell.h"

int is_specific_char(char s)
{
    if ((s >= 'a' && s <= 'z')
        || (s>= 'A' && s <= 'Z')
        || (s >= '0' && s <= '9')
        || s == '_')
        return (1);
    else
        return (0);
}

char *find_var_value(char *var, t_env *envs)
{
    while (envs)
    {
        if (ft_strncmp(var, envs->key, ft_stlen(var)+1) == 0)
            return (envs->value);
        envs = envs->next;
    }
    return (NULL);
}

char    *append_str(char *str, char *s)
{
    char    *new;
    if (!s) s = "";
    new = ft_strjoin(str, s);
    free(str);
    return(new);
}

char    *append_char(char *str, char s)
{
    char new[2];

    new[0] = s;
    new[1] = '\0';
    return (append_str(str, new));
}

char    *expand_var(char *str, int *i, t_env *envs, char *expanded)
{
    int     start;
    char    *var;
    char    *value;

    (*i)++;
    start = *i;
    while (str[*i] && is_specific_char(str[*i]))
        (*i)++;
    if (*i == start)
        return (append_char(expanded, '$'));
    var = ft_substr(str, start, (*i) - start);
    if (!var) return (NULL);
    value = find_var_value(var, envs);
    free(var);
    if (value)
        return (append_str(expanded, value));
    return (expanded); 
}

char    *expansion_string(char *str, t_env *envs)
{
    int     i;
    int     single_quote;
    int     double_quote;
    char    *expanded;
    

    i = 0;
    single_quote = 0;
    double_quote = 0;
    expanded = ft_strdup("");
    if (!expanded) return (NULL);
    while (str[i])
    {
        if (str[i] == '\'' && !double_quote)
        {
            single_quote = !single_quote;
            i++;
            continue;
        }
        if (str[i] == '"' && !single_quote)
        {
            double_quote = !double_quote;
            i++;
            continue;
        }
        if (str[i] == '$' && !single_quote)
        {
            expanded = expand_var(str, &i, envs, expanded);
            if (!expanded) return (NULL);
            continue;
        }
        expanded = append_char(expanded, str[i]);
        if (!expanded) return (NULL);
        i++;        
    }
    return (expanded);
}

void    expansion_args(char **args, t_env *envs)
{
    int i;
    char *expanded;

    i = 0;
    while (args[i])
    {
        expanded = expansion_string(args[i], envs);
        if (!expanded) return;
        free(args[i]);
        args[i] = expanded;
        i++;
    }
}

void    expansion_redir(t_redir *redir, t_env *envs)
{
    char    *expanded;
    
    while (redir)
    {
        expanded = expansion_string(redir->filename, envs);
        if (!expanded) return (NULL);
        free(redir->filename);
        redir->filename = expanded;
        redir = redir->next;
    }
}

void    expansion(t_shell *shell)
{
    t_cmd   *cmds;
    t_env   *envs;

    cmds = shell->cmds;
    envs = shell->env;
    while (cmds)
    {
        expansion_args(cmds->args, envs);
        expansion_redir(cmds->redirects, envs);
        cmds = cmds->next;
    }
}